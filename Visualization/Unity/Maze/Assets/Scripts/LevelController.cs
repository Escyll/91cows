using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using QuickType;
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

    public string GroundTilemapName = "Ground Tilemap";
    public string WallTilemapName = "Wall Tilemap";

    private Tilemap mGroundTilemap;
    private TilemapRenderer mGroundTilemapRenderer;
    private Tilemap mWallTilemap;
    private TilemapRenderer mWallTilemapRenderer;

    private void Start()
    {
        var grid = gameObject.GetComponent<Grid>();

        mGroundTilemap = grid.GetComponentsInChildren<Tilemap>().Single(c => c.name == GroundTilemapName);
        mGroundTilemapRenderer = grid.GetComponentsInChildren<TilemapRenderer>().Single(c => c.name == WallTilemapName);

        mWallTilemap = grid.GetComponentsInChildren<Tilemap>().Single(c => c.name == "Wall Tilemap");
        mWallTilemapRenderer = grid.GetComponentsInChildren<TilemapRenderer>().Single(c => c.name == "Wall Tilemap");

#if DEBUG
        JsonData.text = @"C:\Projects\91cows\JsonGenerator\tick0.json";
        JsonDataFolder.text = @"C:\Projects\91cows\JsonGenerator\";
#endif
    }

    public InputField JsonData;
    public InputField JsonDataFolder;

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

    private System.Drawing.Size mCurrentMapSize = System.Drawing.Size.Empty;

    public GameObject BotPrefab;
    private Dictionary<long, GameObject> mBotLut = new Dictionary<long, GameObject>();

    private void RenderJsonModel(Model model)
    {
        var mapSize = new System.Drawing.Size((int)model.Columns, (int)model.Rows);
        bool mapSizeChanged = mapSize != mCurrentMapSize;
        mCurrentMapSize = mapSize;

        if (mapSizeChanged)
        {
            mGroundTilemap.ClearAllTiles();
            mGroundTilemap.size = new Vector3Int(mapSize.Width, mapSize.Height, 0);
        }

        mWallTilemap.ClearAllTiles();
        mWallTilemap.size = new Vector3Int(mapSize.Width, mapSize.Height, 0);

        for (int row = 0; row < mGroundTilemap.size.y; ++row)
        {
            for (int col = 0; col < mGroundTilemap.size.x; ++col)
            {
                if (mapSizeChanged)
                {
                    mGroundTilemap.SetTile(new Vector3Int(col, row, 1), FloorTile);
                    mGroundTilemap.SetTransformMatrix(new Vector3Int(col, row, 1), GetRandomGroundTileOrientationMatrix());
                }
                mWallTilemap.SetTile(new Vector3Int(col, row, 1), GetWallTile(model.GroundTiles[row][col]));
                mWallTilemap.SetTransformMatrix(new Vector3Int(col, row, 1), GetWallTileOrientationMatrix(model.GroundTiles[row][col]));
            }
        }

        mWallTilemap.ResizeBounds();
        if (mapSizeChanged)
        {
            mGroundTilemap.ResizeBounds();

            float screenRatio = Camera.main.aspect;
            float targetRatio = mGroundTilemapRenderer.bounds.size.x / mGroundTilemapRenderer.bounds.size.y;
            var cam = Camera.main;
            if (screenRatio >= targetRatio)
                cam.orthographicSize = mGroundTilemapRenderer.bounds.size.y / 2;
            else
                cam.orthographicSize = mGroundTilemapRenderer.bounds.size.y / 2 * (targetRatio / screenRatio);

            var size = mGroundTilemapRenderer.bounds.size;
            float ts = cam.pixelHeight / (cam.orthographicSize * 2f);
            float xTiles = cam.pixelWidth / ts;
            cam.transform.position = new Vector3(xTiles / 2, size.y / 2, cam.transform.position.z);
        }

        foreach (var bot in model.Bots)
        {
            if (mBotLut.ContainsKey(bot.ArucoId))
            {
                mBotLut[bot.ArucoId].transform.position = BotToMapPosition(bot, mapSize);
                mBotLut[bot.ArucoId].transform.rotation = GetBotQuaternion(bot);
            }
            else
            {
                Debug.Log($"Instantiating bot {bot.ArucoId}");
                mBotLut[bot.ArucoId] = Instantiate(BotPrefab, BotToMapPosition(bot, mapSize), Quaternion.identity);
                mBotLut[bot.ArucoId].GetComponent<Renderer>().material.SetColor("_Color", Color.green);
                mBotLut[bot.ArucoId].transform.rotation = GetBotQuaternion(bot);
            }
        }
    }

    private static Quaternion GetBotQuaternion(Bot bot)
    {
        Vector3 forward = new Vector3(0f, (float)(bot.Forward[0]), (float)(bot.Forward[1]));
        Vector3 right = new Vector3(0f, (float)bot.Right[0], (float)bot.Right[1]);
        //Debug.Log($"Forward {forward}, Right {right}");
        Vector3 rightProjected = Vector3.ProjectOnPlane(forward, right).normalized; // ensure vectors are perpendicular
        return Quaternion.LookRotation(forward, rightProjected);
    }

    private static Vector3 BotToMapPosition(Bot bot, System.Drawing.Size mapSize)
    {
        return new Vector3((float)bot.Position[0] * mapSize.Width, (float)bot.Position[1] * mapSize.Height, 0);
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
}
