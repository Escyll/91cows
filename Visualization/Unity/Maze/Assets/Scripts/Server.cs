using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class Server : IDisposable
{
    public Server(IPAddress ipAddress, int port = 4242)
    {
        mIPAddress = ipAddress;
        mPort = port;
    }

    public void Dispose()
    {
        Close();
    }

    private readonly byte[] mBuff = new byte[8196];
    private readonly IPAddress mIPAddress;
    private readonly int mPort;
    private Thread mThread;

    public void Open()
    {
        if (mThread != null)
            return;

        System.Diagnostics.Debug.Assert(mSocketTarget == null);
        SetupConnection();
        mThread = new Thread(new ThreadStart(Run));
        mThread.Start();
    }

    public void Close()
    {
        mThread?.Abort();
        mThread?.Join();
        mThread = null;
        System.Diagnostics.Debug.Assert(mSocketTarget == null);
    }

    private void SetupConnection()
    {
        var socket = new Socket(mIPAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
        socket.Connect(new IPEndPoint(mIPAddress, mPort));
        Debug.Log($"Socket connected to {mSocketTarget.RemoteEndPoint.ToString()}");
        mSocketTarget = socket;
    }

    private Socket mSocketTarget;

    private void Run()
    {
        while (true)
        {
            try
            {
                int bytesRec = mSocketTarget.Receive(mBuff);
                var json = Encoding.ASCII.GetString(mBuff, 0, bytesRec);
                Debug.Log($"Server received JSON: {json}");
                OnModelReceived?.Invoke(Model.FromJson(json));
            }
            catch (ThreadAbortException)
            {
                Debug.Log("Server disconnecting");
                mSocketTarget.Shutdown(SocketShutdown.Both);
                mSocketTarget.Close(3);
                mSocketTarget = null;
                Debug.Log("Server disconnected");
            }
            catch (Exception ex)
            {
                Debug.LogError($"Server error: {ex.Message}");
            }
        }
    }

    public delegate void ModelReceived(Model model);
    public event ModelReceived OnModelReceived;
}
