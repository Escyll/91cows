// To parse this JSON data, add NuGet 'Newtonsoft.Json' then do:
//
//    using QuickType;
//
//    var model = Model.FromJson(jsonString);

namespace QuickType
{
    using System.Globalization;
    using Newtonsoft.Json;
    using Newtonsoft.Json.Converters;

    public partial class Model
    {
        [JsonProperty("gameId")]
        public long GameId { get; set; }

        [JsonProperty("gameTick")]
        public long GameTick { get; set; }

        [JsonProperty("rows")]
        public long Rows { get; set; }

        [JsonProperty("columns")]
        public long Columns { get; set; }

        [JsonProperty("data")]
        public GroundTile[][] GroundTiles { get; set; }

        [JsonProperty("actionTiles")]
        public ActionTile[] ActionTiles { get; set; }

        [JsonProperty("bots")]
        public Bot[] Bots { get; set; }
    }

    public partial class ActionTile
    {
        [JsonProperty("x")]
        public long X { get; set; }

        [JsonProperty("y")]
        public long Y { get; set; }

        [JsonProperty("direction")]
        public string Direction { get; set; }

        [JsonProperty("laneId")]
        public long LaneId { get; set; }
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
        public static Model FromJson(string json) => JsonConvert.DeserializeObject<Model>(json, QuickType.Converter.Settings);
    }

    public static class Serialize
    {
        public static string ToJson(this Model self) => JsonConvert.SerializeObject(self, QuickType.Converter.Settings);
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
}
