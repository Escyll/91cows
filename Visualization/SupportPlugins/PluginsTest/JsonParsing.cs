using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PluginsTest
{
    [TestFixture]
    public class JsonParsing
    {
        [Test]
        public void ParseJsonString()
        {
            string jsonData = "{ 'gameId': 1, 'gameTick': 3, 'rows': 10, 'columns': 10, 'data': [[{'type': 1, 'orientation': 0}, {'type': 0, 'orientation': 0}, {'type': 1, 'orientation': 270}, {'type': 0, 'orientation': 270}, {'type': 0, 'orientation': 90}, {'type': 0, 'orientation': 180}, {'type': 1, 'orientation': 270}, {'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 270}, {'type': 0, 'orientation': 180}], [{'type': 1, 'orientation': 270}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 180}, {'type': 0, 'orientation': 270}, {'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 180}, {'type': 1, 'orientation': 270}, {'type': 0, 'orientation': 180}, {'type': 0, 'orientation': 270}, {'type': 1, 'orientation': 0}], [{'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 270}, {'type': 1, 'orientation': 180}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 0}], [{'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 1, 'orientation': 180}, {'type': 0, 'orientation': 270}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 0}, {'type': 1, 'orientation': 0}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 180}, {'type': 1, 'orientation': 180}], [{'type': 0, 'orientation': 180}, {'type': 0, 'orientation': 180}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 180}, {'type': 0, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 270}, {'type': 1, 'orientation': 270}], [{'type': 0, 'orientation': 90}, {'type': 0, 'orientation': 180}, {'type': 1, 'orientation': 0}, {'type': 0, 'orientation': 270}, {'type': 1, 'orientation': 270}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 270}, {'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 180}, {'type': 1, 'orientation': 90}], [{'type': 0, 'orientation': 0}, {'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 270}, {'type': 1, 'orientation': 270}, {'type': 1, 'orientation': 180}, {'type': 0, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 0}], [{'type': 1, 'orientation': 0}, {'type': 1, 'orientation': 90}, {'type': 1, 'orientation': 180}, {'type': 1, 'orientation': 180}, {'type': 1, 'orientation': 90}, {'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 1, 'orientation': 180}, {'type': 1, 'orientation': 180}, {'type': 0, 'orientation': 90}], [{'type': 1, 'orientation': 0}, {'type': 1, 'orientation': 270}, {'type': 1, 'orientation': 270}, {'type': 0, 'orientation': 90}, {'type': 1, 'orientation': 270}, {'type': 1, 'orientation': 0}, {'type': 1, 'orientation': 0}, {'type': 0, 'orientation': 90}, {'type': 0, 'orientation': 180}, {'type': 0, 'orientation': 90}], [{'type': 0, 'orientation': 90}, {'type': 0, 'orientation': 270}, {'type': 0, 'orientation': 270}, {'type': 1, 'orientation': 180}, {'type': 0, 'orientation': 180}, {'type': 1, 'orientation': 180}, {'type': 1, 'orientation': 0}, {'type': 1, 'orientation': 90}, {'type': 0, 'orientation': 0}, {'type': 0, 'orientation': 90}]], 'actionTiles': [{'x': 0, 'y': 5, 'direction': 'up', 'laneId': 2}, {'x': 4, 'y': 6, 'direction': 'up', 'laneId': 8}, {'x': 8, 'y': 6, 'direction': 'left', 'laneId': 8}, {'x': 7, 'y': 4, 'direction': 'right', 'laneId': 1}, {'x': 3, 'y': 2, 'direction': 'up', 'laneId': 1}], 'bots': [{'arucoId': 0, 'position': [0.2, 0.6], 'forward': [0.02, -0.03], 'right': [-0.01, 0.04]}]}";
            var model = QuickType.Model.FromJson(jsonData);
            Assert.AreEqual(model.GameId, 1);
            Assert.AreEqual(model.GameTick, 3);
            Assert.AreEqual(model.Rows, 10);
            Assert.AreEqual(model.Columns, 10);
            Assert.AreEqual(model.GroundTiles.SelectMany(t => t).Count(), model.Columns * model.Rows);
            Assert.That(model.GroundTiles.SelectMany(t => t).All(t => (0 <= t.Type && t.Type < 4) && t.Orientation % 90 == 0));
            Assert.That(model.ActionTiles.Length > 0);
            Assert.That(model.Bots.Length > 0);
        }
    }
}
