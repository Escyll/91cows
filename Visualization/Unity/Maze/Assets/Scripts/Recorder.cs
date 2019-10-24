using System;
using System.Collections.Concurrent;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using UnityEngine;

public class Recorder : IDisposable
{

    private readonly string mBasePath = Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
    private string mPath;
    private readonly Thread mThread = null;
    private long mCurrentGameId = -1;
    private long mLastTick = long.MaxValue;

    public Recorder()
    {
        mThread = new Thread(new ThreadStart(Run));
        mThread.Start();
    }

    public void Dispose()
    {
        mThread?.Abort();
        mThread?.Join();
    }

    public void Write(Model model)
    {
        mModels.Enqueue(model);
    }

    private void PrepareOutputPath(long gameId)
    {
        mPath = Path.Combine(mBasePath, "swoc2019", $"{DateTime.Now.ToString("dd-MMMM-yyyy HH_mm_ss")} game_{gameId}");
        if (Directory.Exists(mPath) && Directory.EnumerateFiles(mPath).Any())
        {
            int uniqueId = 0;
            var altPath = $"{mPath} - {++uniqueId}";
            while (Directory.Exists(altPath) && Directory.EnumerateFiles(altPath).Any())
                altPath = $"{mPath} - {++uniqueId}";
            mPath = altPath;
        }
        if (!Directory.Exists(mPath))
            Directory.CreateDirectory(mPath);
    }

    private readonly ConcurrentQueue<Model> mModels = new ConcurrentQueue<Model>();

    private void Run()
    {
        try
        {
            while (true)
            {
                if (mModels.TryDequeue(out Model model))
                    Save(model);
                else
                    Thread.Sleep(100);
            }
        }
        catch (ThreadAbortException)
        {
            try
            {
                while (mModels.TryDequeue(out Model model))
                    Save(model);
            }
            catch (Exception ex)
            {
                Debug.Log("Recorder exception: " + ex.Message);
            }
        }
        catch (Exception ex)
        {
            Debug.Log("Recorder exception: " + ex.Message);
        }
    }

    private void Save(Model model)
    {
        if (mCurrentGameId != model.GameId || model.GameTick < mLastTick)
            PrepareOutputPath(model.GameId);
        SaveToOutputPath(model);
        mCurrentGameId = model.GameId;
        mLastTick = model.GameTick;
    }

    private void SaveToOutputPath(Model model)
    {
        File.WriteAllText(Path.Combine(mPath, $"tick{model.GameTick}.json"), model.ToJson(), Encoding.ASCII);
    }
}
