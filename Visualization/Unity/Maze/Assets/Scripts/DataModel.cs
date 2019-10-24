using System.Globalization;
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;

public enum GameState
{
    Waiting = 0,
    Running = 1,
    Stopped = 2
};

public partial class Model
{
    [JsonProperty("gameId")]
    public long GameId { get; set; }

    [JsonProperty("gameTick")]
    public long GameTick { get; set; }

    [JsonProperty("gameState")]
    [JsonConverter(typeof(StringEnumConverter))]
    public GameState GameState { get; set; }

    [JsonProperty("rows")]
    public long Rows { get; set; }

    [JsonProperty("columns")]
    public long Columns { get; set; }

    [JsonProperty("data")]
    public GroundTile[][] GroundTiles { get; set; }

    [JsonProperty("actionItems")]
    public ActionItem[] ActionItems { get; set; }

    [JsonProperty("bots")]
    public Bot[] Bots { get; set; }
}

public enum ActionItemType
{
    Coin = 0,
    TreasureChest = 1,
    EmptyChest = 2,
    MimicChest = 3,
    SpikeTrap = 4,
    Bottle = 5,
    TestTube = 6
}

public partial class ActionItem
{
    [JsonProperty("id")]
    public long Id { get; set; }

    [JsonProperty("x")]
    public double X { get; set; }

    [JsonProperty("y")]
    public double Y { get; set; }

    [JsonProperty("type")]
    [JsonConverter(typeof(StringEnumConverter))]
    public ActionItemType Type { get; set; }
}

public partial class Bot
{
    [JsonProperty("arucoId")]
    public long ArucoId { get; set; }

    [JsonProperty("position")]
    public double[] Position { get; set; }

    [JsonProperty("forward")]
    public double[] Forward { get; set; }

    [JsonProperty("right")]
    public double[] Right { get; set; }

    [JsonProperty("score")]
    public int Score { get; set; }

    [JsonProperty("color")] // interpret as argb UInt32
    public System.Int32 Color { get; set; }

    public UnityEngine.Color UnityColor
    {
        get
        {
            return new UnityEngine.Color(
                (Color >> 16 & 0xff) / 255f, 
                (Color >> 8 & 0xff) / 255f, 
                (Color & 0xff) / 255f, 
                (Color >> 24 & 0xff) / 255f);
        }
    }

    [JsonProperty("name")]
    public string Name { get; set; }
}

public partial class GroundTile
{
    [JsonProperty("type")]
    public long Type { get; set; }

    [JsonProperty("orientation")]
    public long Orientation { get; set; }
}

public partial class Model
{
    public static Model FromJson(string json) => JsonConvert.DeserializeObject<Model>(json, Converter.Settings);
}

public static class Serialize
{
    public static string ToJson(this Model self) => JsonConvert.SerializeObject(self, Converter.Settings);
}

internal static class Converter
{
    public static readonly JsonSerializerSettings Settings = new JsonSerializerSettings
    {
        MetadataPropertyHandling = MetadataPropertyHandling.Ignore,
        DateParseHandling = DateParseHandling.None,
        Converters = {
                new IsoDateTimeConverter { DateTimeStyles = DateTimeStyles.AssumeUniversal }
            },
    };
}
