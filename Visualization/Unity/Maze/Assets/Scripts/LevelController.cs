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

        CreateMap(10, 10);
    }

    private void CreateMap(int mapWidth, int mapHeight)
    {
        mGroundTilemap.ClearAllTiles();
        mGroundTilemap.size = new Vector3Int(mapWidth, mapHeight, 0);

        mWallTilemap.ClearAllTiles();
        mWallTilemap.size = new Vector3Int(mapWidth, mapHeight, 0);

        for (int j = 0; j < mGroundTilemap.size.y; ++j)
        {
            for (int i = 0; i < mGroundTilemap.size.x; ++i)
            {
                mGroundTilemap.SetTile(new Vector3Int(i, j, 1), FloorTile);
                mWallTilemap.SetTile(new Vector3Int(i, j, 1), AllOpenWallTile);
            }
        }

        mGroundTilemap.ResizeBounds();
        mWallTilemap.ResizeBounds();

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

    public InputField MapWidth;
    public InputField MapHeight;

    public void GenerateMap()
    {
        CreateMap(int.Parse(MapWidth.text), int.Parse(MapHeight.text));
    }
}
