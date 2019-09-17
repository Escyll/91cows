using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
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

    public InputField IpAddress;
    public InputField JsonData;
    public InputField JsonDataFolder;

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

    private void Start()
    {
#if DEBUG
        JsonData.text = @"C:\Projects\91cows\JsonGenerator\tick0.json";
        JsonDataFolder.text = @"C:\Projects\91cows\JsonGenerator\";
        IpAddress.text = @"127.0.0.1";
#endif
    }

    private void Update()
    {
        if (mServer != null)
        {
            if (mModels.TryDequeue(out Model model))
                RenderJsonModel(model);
        }
    }

    #region Server

    private Server mServer;

    public void ConnectToServer()
    {
        mServer?.Dispose();
        mServer = null;

        var server = new Server(System.Net.IPAddress.Parse(IpAddress.text));
        server.Open();
        mServer = server;
        ResetBotLut();
    }

    private void OnServerModelRecieved(Model model)
    {
        mModels.Enqueue(model);
    }

    private ConcurrentQueue<Model> mModels;

    #endregion

    #region Render map logic

    private Vector2Int mCurrentMapSize = Vector2Int.zero;

    private Dictionary<long, GameObject> mBotLut = new Dictionary<long, GameObject>();

    private void RenderJsonModel(Model model)
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

            float screenRatio = Camera.main.aspect;
            float targetRatio = GroundTilemapRenderer.bounds.size.x / GroundTilemapRenderer.bounds.size.y;
            var cam = Camera.main;
            if (screenRatio >= targetRatio)
                cam.orthographicSize = GroundTilemapRenderer.bounds.size.y / 2;
            else
                cam.orthographicSize = GroundTilemapRenderer.bounds.size.y / 2 * (targetRatio / screenRatio);

            var size = GroundTilemapRenderer.bounds.size;
            float ts = cam.pixelHeight / (cam.orthographicSize * 2f);
            float xTiles = cam.pixelWidth / ts;
            cam.transform.position = new Vector3(xTiles / 2, size.y / 2, cam.transform.position.z);
        }

        foreach (var bot in model.Bots)
        {
            //bot.Position = new double[] { 0.5, 0.5 };
            bot.Forward = new double[] { 1, 1 };
            bot.Right = new double[] { 0.5, -0.5 };

            if (mBotLut.ContainsKey(bot.ArucoId))
            {
                mBotLut[bot.ArucoId].transform.position = BotToMapPosition(bot, mapSize);
                var sprite = GetBotSprite(bot);
                sprite.up = MakeVector(bot.Forward);
                sprite.right = MakeVector(bot.Right);
                mBotLut[bot.ArucoId].transform.localScale = new Vector3(sprite.right.magnitude, sprite.up.magnitude);

#if DEBUG
                Debug.DrawRay(sprite.position, sprite.right, Color.green);
                Debug.DrawRay(sprite.position, sprite.up, Color.yellow);
#endif
            }
            else
            {
                Debug.Log($"Instantiating bot {bot.ArucoId}");
                mBotLut[bot.ArucoId] = Instantiate(BotPrefab, BotToMapPosition(bot, mapSize), Quaternion.identity);
                SetBotColor(bot, new Color(1, 1, 0, 1));
                Transform sprite = GetBotSprite(bot);
                sprite.up = MakeVector(bot.Forward);
                sprite.right = MakeVector(bot.Right);
                mBotLut[bot.ArucoId].transform.localScale = new Vector3(sprite.right.magnitude, sprite.up.magnitude);
            }
        }
    }

    private void SetBotColor(Bot bot, Color color)
    {
        mBotLut[bot.ArucoId].GetComponentInChildren<Renderer>().material.SetColor("_Color", color);
    }

    private Transform GetBotSprite(Bot bot)
    {
        return mBotLut[bot.ArucoId].transform.Find("Origin").transform.Find("Sprite");
    }

    private static Vector3 MakeVector(double[] pointArrayVector)
    {
        return new Vector3((float)pointArrayVector[0], (float)pointArrayVector[1], 0f);
    }

    private static Vector3 BotToMapPosition(Bot bot, Vector2Int mapSize)
    {
        Debug.Log(new Vector3((float)bot.Position[0] * (mapSize.x - 1), (float)bot.Position[1] * (mapSize.y - 1), 0));
        return new Vector3((float)bot.Position[0] * (mapSize.x - 1), (float)bot.Position[1] * (mapSize.y - 1), 0);
    }

    private Tile GetWallTile(GroundTile groundTile)
    {
        switch (groundTile.Type)
        {
            case 0:
                return AllOpenWallTile;
            case 1:
                return StraightWallTile;
            case 2:
                return TCornerWallTile;
            case 3:
                return LCornerWallTile;
            default:
                throw new ArgumentException("Unknown ground tile type");
        }
    }

    private static Matrix4x4 GetWallTileOrientationMatrix(GroundTile groundTile)
    {
        return Matrix4x4.TRS(new Vector3(0, 0, 0), Quaternion.Euler(0f, 0f, groundTile.Orientation), new Vector3(1, 1, 1));
    }

    private readonly System.Random mRand = new System.Random(DateTime.Now.TimeOfDay.Milliseconds);

    private Matrix4x4 GetRandomGroundTileOrientationMatrix()
    {
        return Matrix4x4.TRS(new Vector3(0, 0, 0), Quaternion.Euler(0f, 0f, mRand.Next(0, 4) * 90f), new Vector3(1, 1, 1));
    }

    #endregion

    #region Render from local datasource

    // generate from single json
    public void GenerateMapFromJson()
    {
        if (!mIsRunning && File.Exists(JsonData.text))
        {
            ResetBotLut();
            RenderJsonModel(Model.FromJson(File.ReadAllText(JsonData.text)));
        }
        else
        {
            Debug.Log($"JSON file '{JsonData.text}' not found or already running from folder");
        }
    }

    private void ResetBotLut()
    {
        foreach (var kvp in mBotLut)
        {
            Debug.Log($"Destroying bot {kvp.Key}");
            Destroy(kvp.Value, 5);
        }
        mBotLut.Clear();
    }

    private bool mIsRunning = false;
    private string mJsonFolder;
    private int mTick;

    public void RunMapFromJsonFolder()
    {
        if (!mIsRunning && Directory.Exists(JsonDataFolder.text))
        {
            ResetBotLut();
            mJsonFolder = JsonDataFolder.text;
            mIsRunning = true;
            mTick = 0;
            InvokeRepeating(nameof(RenderNextFolderTick), 0, 1 / 30f);
        }
        else
        {
            Debug.Log($"JSON folder '{JsonDataFolder.text}' not found or already running");
        }
    }

    private void RenderNextFolderTick()
    {
        var filename = Path.Combine(mJsonFolder, $"tick{mTick++}.json");
        if (File.Exists(filename))
        {
            RenderJsonModel(Model.FromJson(File.ReadAllText(filename)));
        }
        else
        {
            mIsRunning = false;
            ResetBotLut();
            CancelInvoke(nameof(RenderNextFolderTick));
        }
    }

    #endregion

}
