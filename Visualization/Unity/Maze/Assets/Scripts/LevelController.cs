using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEngine.Tilemaps;
using UnityEngine.UI;

public class LevelController : MonoBehaviour
{
    public Tile FloorTile;
    public Tile AllOpenWallTile;
    public Tile StraightWallTile;
    public Tile LCornerWallTile;
    public Tile TCornerWallTile;
    public Tile DeadendTile;

    public InputField IpAddress;
    public InputField JsonDataSource;
    public InputField JsonDataSourceFps;
    public Toggle RecordToggle;

    public Tilemap GroundTilemap;
    public TilemapRenderer GroundTilemapRenderer;
    public Tilemap WallTilemap;
    public TilemapRenderer WallTilemapRenderer;

    public GameObject BotPrefab;
    public GameObject CoinPrefab;
    public GameObject TesttubePrefab;
    public GameObject FlaskPrefab;
    public GameObject SpiketrapPrefab;
    public GameObject EmptyTreasureChestPrefab;
    public GameObject FullTreasureChestPrefab;
    public GameObject MimicTreasureChestPrefab;

    public GameObject ScoreScrollContent;
    public GameObject ScoreItemPrefab;
    public Text GameInfo;

    #region Unity

    private void Start()
    {
#if DEBUG
        JsonDataSource.text = @"C:\Projects\Swoc2019\JsonGenerator\tick0.json";
        IpAddress.text = @"10.10.0.37";
#endif
        JsonDataSourceFps.text = "30";
    }

    private void Update()
    {
        if (mServer != null && mModels.TryDequeue(out Model model))
        {
            RenderJsonModel(model);
            mRecorder?.Write(model);
        }
    }

    private void OnApplicationQuit()
    {
        if (mServer != null)
        {
            mServer.OnModelReceived -= OnServerModelReceived;
            mServer.Dispose();
            mServer = null;
        }
        mRecorder?.Dispose();
        mRecorder = null;
    }

    #endregion

    #region Server

    private Recorder mRecorder;
    private Server mServer;
    private ConcurrentQueue<Model> mModels = new ConcurrentQueue<Model>();

    public void ConnectToServer()
    {
        DisconnectFromServer();
        var server = new Server(System.Net.IPAddress.Parse(IpAddress.text));
        server.Open();
        mServer = server;
        mServer.OnModelReceived += OnServerModelReceived;
        if (RecordToggle.isOn)
            mRecorder = new Recorder();
    }

    public void DisconnectFromServer()
    {
        if (mServer != null)
            mServer.OnModelReceived -= OnServerModelReceived;
        mServer?.Dispose();
        mServer = null;
        ResetGameData();
        mRecorder?.Dispose();
        mRecorder = null;
    }

    private void OnServerModelReceived(Model model)
    {
        mModels.Enqueue(model);
    }

    #endregion

    #region Game render logic

    private Vector2Int mCurrentMapSize = Vector2Int.zero;
    private Dictionary<long, GameObject> mBotLut = new Dictionary<long, GameObject>();
    private Dictionary<long, GameObject> mItems = new Dictionary<long, GameObject>();
    private Dictionary<long, GameObject> mScoreItems = new Dictionary<long, GameObject>();

    private void RenderJsonModel(Model model)
    {
        RenderGameStats(model);
        RenderMap(model);
        RenderBots(model);
        RenderItems(model);
    }

    #region Game Stats

    private void RenderGameStats(Model model)
    {
        GameInfo.text = $"Game {model.GameId}: {GameStateDescriptor(model)}";
    }

    private static string GameStateDescriptor(Model model)
    {
        return model.GameState == GameState.Running
            ? model.GameTick.ToString()
            : model.GameState == GameState.Stopped
                ? "stopped"
                : "waiting";
    }

    #endregion

    #region Map

    private void RenderMap(Model model)
    {
        var mapSize = new Vector2Int((int)model.Columns, (int)model.Rows);
        bool mapSizeChanged = mapSize != mCurrentMapSize;
        mCurrentMapSize = mapSize;

        if (mapSizeChanged)
        {
            GroundTilemap.ClearAllTiles();
            GroundTilemap.size = new Vector3Int(mapSize.x, mapSize.y, 0);
        }

        WallTilemap.ClearAllTiles();
        WallTilemap.size = new Vector3Int(mapSize.x, mapSize.y, 0);

        for (int row = 0; row < GroundTilemap.size.y; ++row)
        {
            for (int col = 0; col < GroundTilemap.size.x; ++col)
            {
                if (mapSizeChanged)
                {
                    GroundTilemap.SetTile(new Vector3Int(col, row, 1), FloorTile);
                    GroundTilemap.SetTransformMatrix(new Vector3Int(col, row, 1), GetRandomGroundTileOrientationMatrix());
                }
                WallTilemap.SetTile(new Vector3Int(col, row, 1), GetWallTile(model.GroundTiles[row][col]));
                WallTilemap.SetTransformMatrix(new Vector3Int(col, row, 1), GetWallTileOrientationMatrix(model.GroundTiles[row][col]));
            }
        }

        WallTilemap.ResizeBounds();
        if (mapSizeChanged)
        {
            GroundTilemap.ResizeBounds();
            Camera cam = Camera.main;
            cam.orthographicSize = FitOrthographicSize();
            mZoomedOutOrthoSize = cam.orthographicSize;
            var size = GroundTilemapRenderer.bounds.size;
            //float ts = cam.pixelHeight / (cam.orthographicSize * 2f);
            //float xTiles = cam.pixelWidth / ts;
            cam.transform.position = new Vector3(size.x / 2, size.y / 2, cam.transform.position.z);
            mZoomedOutCamPos = cam.transform.position;
        }
    }

    private float mZoomedOutOrthoSize = 1;
    private Vector3 mZoomedOutCamPos = new Vector3();

    private float FitOrthographicSize()
    {
        float screenRatio = Camera.main.aspect;
        float targetRatio = GroundTilemapRenderer.bounds.size.x / GroundTilemapRenderer.bounds.size.y;
        return (screenRatio >= targetRatio)
            ? GroundTilemapRenderer.bounds.size.y / 2
            : GroundTilemapRenderer.bounds.size.y / 2 * (targetRatio / screenRatio);
    }

    private Tile GetWallTile(GroundTile groundTile)
    {
        switch (groundTile.Type)
        {
            case 0:
                return AllOpenWallTile;
            case 1:
                return TCornerWallTile;
            case 2:
                return StraightWallTile;
            case 3:
                return LCornerWallTile;
            case 4:
                return DeadendTile;
            default:
                throw new ArgumentException("Unknown ground tile type");
        }
    }

    private static Matrix4x4 GetWallTileOrientationMatrix(GroundTile groundTile)
    {
        return Matrix4x4.TRS(new Vector3(0, 0, 0), Quaternion.Euler(0f, 0f, groundTile.Orientation), new Vector3(1, 1, 1));
    }

    private Matrix4x4 GetRandomGroundTileOrientationMatrix()
    {
        return Matrix4x4.TRS(new Vector3(0, 0, 0), Quaternion.Euler(0f, 0f, mRand.Next(0, 4) * 90f), new Vector3(1, 1, 1));
    }

    #endregion

    #region Bot

    private void RenderBots(Model model)
    {
        ClearScoreItems();
        foreach (var bot in model.Bots)
        {
            if (mBotLut.ContainsKey(bot.ArucoId))
            {
                UpdateBot(bot);
#if DEBUG
                var sprite = GetBotSprite(bot);
                Debug.DrawRay(sprite.position, sprite.right, Color.green);
                Debug.DrawRay(sprite.position, sprite.up, Color.yellow);
#endif
            }
            else
            {
                InstantiateBot(bot);
                UpdateBot(bot);
            }

            InstantiateScoreItem(bot);
            //UpdateScoreItem(bot);
        }
    }

    private void InstantiateBot(Bot bot)
    {
        Debug.Log($"Instantiating bot {bot.ArucoId}");
        mBotLut[bot.ArucoId] = Instantiate(BotPrefab, GroundTilemap.transform);
        SetBotColor(bot, GetBotColor(bot));
        mBotLut[bot.ArucoId].transform.localScale = GetBotScale(bot);
    }

    private void UpdateBot(Bot bot)
    {
        mBotLut[bot.ArucoId].transform.rotation = GetBotQuaternion(bot);
        mBotLut[bot.ArucoId].transform.position = BotToMapPosition(bot);
    }

    private static Quaternion GetBotQuaternion(Bot bot)
    {
        Vector2 forward = new Vector2((float)(bot.Forward[0]), (float)(bot.Forward[1]));
        return Quaternion.AngleAxis(Vector2.SignedAngle(new Vector2(0, 1), forward), new Vector3(0, 0, 1));
    }

    private Color GetBotColor(Bot bot)
    {
        return bot.UnityColor;
    }

    private void SetBotColor(Bot bot, Color color)
    {
        mBotLut[bot.ArucoId].GetComponentInChildren<Renderer>().material.SetColor("_Color", color);
    }

    private Transform GetBotSprite(Bot bot)
    {
        return mBotLut[bot.ArucoId].transform.Find("Origin").transform.Find("Sprite");
    }

    private Vector3 GetBotScale(Bot bot)
    {
        var forward = MakeVector(bot.Forward);
        var right = MakeVector(bot.Right);
        return new Vector3((right.magnitude * mCurrentMapSize.x) * 2, (forward.magnitude * mCurrentMapSize.y) * 2);
    }

    private Vector3 BotToMapPosition(Bot bot)
    {
        return new Vector3((float)bot.Position[0] * mCurrentMapSize.x, (float)bot.Position[1] * mCurrentMapSize.y, 0);
    }

    #endregion

    #region Items

    private void RenderItems(Model model)
    {
        var actionItems = model.ActionItems ?? new ActionItem[0];
        foreach (var item in actionItems)
        {
            if (!mItems.ContainsKey(item.Id))
                mItems[item.Id] = Instantiate(GetItemPrefab(item.Type), ItemToMapPosition(item), Quaternion.identity, GroundTilemap.transform);
        }

        var keysToRemove = new List<long>();
        foreach (var item in mItems)
        {
            if (!actionItems.Any(p => p.Id == item.Key))
            {
                Destroy(item.Value);
                keysToRemove.Add(item.Key);
            }
        }

        foreach (var id in keysToRemove)
            mItems.Remove(id);
    }

    private GameObject GetItemPrefab(ActionItemType type)
    {
        switch (type)
        {
            case ActionItemType.Coin:
                return CoinPrefab;
            case ActionItemType.TreasureChest:
                return FullTreasureChestPrefab;
            case ActionItemType.EmptyChest:
                return EmptyTreasureChestPrefab;
            case ActionItemType.MimicChest:
                return MimicTreasureChestPrefab;
            case ActionItemType.SpikeTrap:
                return SpiketrapPrefab;
            case ActionItemType.Bottle:
                return FlaskPrefab;
            case ActionItemType.TestTube:
                return TesttubePrefab;
            default:
                return CoinPrefab;
        }
    }

    private Vector3 ItemToMapPosition(ActionItem item)
    {
        return new Vector3((float)item.X * mCurrentMapSize.x, (float)item.Y * mCurrentMapSize.y, 0);
    }

    #endregion

    #region Scoreboard

    private void InstantiateScoreItem(Bot bot)
    {
        System.Diagnostics.Debug.Assert(!mScoreItems.ContainsKey(bot.ArucoId));
        var scoreItem = Instantiate(ScoreItemPrefab, ScoreScrollContent.transform);
        var background = scoreItem.transform.Find("Image");
        background.GetComponentInChildren<Image>().color = GetBotColor(bot);
        var name = scoreItem.transform.Find("Name");
        name.GetComponentInChildren<Text>().text = $"{bot.Name} ({bot.ArucoId})";
        var score = scoreItem.transform.Find("Score");
        score.GetComponentInChildren<Text>().text = $"{bot.Score} points";
        mScoreItems.Add(bot.ArucoId, scoreItem);
    }

    private void UpdateScoreItem(Bot bot)
    {
        System.Diagnostics.Debug.Assert(mScoreItems.ContainsKey(bot.ArucoId));
        var scoreItem = mScoreItems[bot.ArucoId];
        var background = scoreItem.transform.Find("Image");
        background.GetComponentInChildren<Image>().color = GetBotColor(bot);
        var name = scoreItem.transform.Find("Name");
        name.GetComponentInChildren<Text>().text = bot.Name;
        var score = scoreItem.transform.Find("Score");
        score.GetComponentInChildren<Text>().text = $"{bot.Score} points";
    }

    #endregion

    #region Render helpers

    private readonly System.Random mRand = new System.Random(DateTime.Now.TimeOfDay.Milliseconds);

    private static Vector3 MakeVector(double[] pointArrayVector)
    {
        return new Vector3((float)pointArrayVector[0], (float)pointArrayVector[1], 0f);
    }


    private void ResetGameData()
    {
        foreach (var item in mBotLut)
        {
            Debug.Log($"Destroying bot {item.Key}");
            Destroy(item.Value);
        }
        mBotLut.Clear();

        foreach (var item in mItems)
        {
            Debug.Log($"Destroying item {item.Key}");
            Destroy(item.Value);
        }
        mItems.Clear();

        ClearScoreItems();

        mCurrentMapSize = new Vector2Int(0, 0);

        GroundTilemap.ClearAllTiles();
        WallTilemap.ClearAllTiles();

        GameInfo.text = "";
    }

    private void ClearScoreItems()
    {
        foreach (var item in mScoreItems)
        {
            //Debug.Log($"Destroying scoreitem {item.Key}");
            Destroy(item.Value);
        }
        mScoreItems.Clear();
    }

    #endregion

    #endregion

    #region Render from local datasource

    private bool mIsRunning = false;
    private int mTick = 0;
    private string mJsonFolder;

    public void RenderFromLocalJsonSource()
    {
        if (mIsRunning)
        {
            CancelRenderFromFolder();
        }
        else if (File.Exists(JsonDataSource.text) && Path.GetExtension(JsonDataSource.text) == ".json")
        {
            RenderJsonFromFile(JsonDataSource.text);
        }
        else if (Directory.Exists(JsonDataSource.text))
        {
            StartRenderFromFolder();
        }
        else
        {
            Debug.Log($"JSON source '{JsonDataSource.text}' not available");
        }
    }

    private void StartRenderFromFolder()
    {
        mJsonFolder = JsonDataSource.text;
        mIsRunning = true;
        float fps = int.Parse(JsonDataSourceFps.text);
        InvokeRepeating(nameof(RenderNextFolderTick), 0, 1 / fps);
    }

    private void RenderJsonFromFile(string filename)
    {
        RenderJsonModel(Model.FromJson(File.ReadAllText(filename)));
    }

    private void RenderNextFolderTick()
    {
        var filename = Path.Combine(mJsonFolder, $"tick{mTick++}.json");
        if (File.Exists(filename))
        {
            RenderJsonFromFile(filename);
        }
        else
        {
            CancelRenderFromFolder();
        }
    }

    private void CancelRenderFromFolder()
    {
        mTick = 0;
        mIsRunning = false;
        ResetGameData();
        CancelInvoke(nameof(RenderNextFolderTick));
    }

    #endregion
}
