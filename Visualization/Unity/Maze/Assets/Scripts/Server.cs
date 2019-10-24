using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;

public class Server : IDisposable
{
    public Server(IPAddress ipAddress, int port = 9835)
    {
        mIPAddress = ipAddress;
        mPort = port;
    }

    public void Dispose()
    {
        Close();
    }

    private readonly byte[] mBuff = new byte[65536];
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
        Debug.Log($"Socket connected to {socket.RemoteEndPoint.ToString()}");
        mSocketTarget = socket;
    }

    private Socket mSocketTarget;

    private void Run()
    {
        int idx = -1;
        string dataPacket = "";
        while (true)
        {
            try
            {
                int bytesRecv = mSocketTarget.Receive(mBuff, mBuff.Length, SocketFlags.None);
                var data = Encoding.ASCII.GetString(mBuff, 0, bytesRecv);

                int offset = 0;
                while (offset < data.Length && (idx = data.IndexOf('\n', offset)) != -1)
                {
                    dataPacket += data.Substring(offset, idx + 1 - offset);
                    offset = idx + 1;
                    OnModelReceived?.Invoke(Model.FromJson(dataPacket));
                    //Debug.Log($"Server received JSON: {json}");
                    dataPacket = "";
                }

                if (offset < data.Length)
                    dataPacket = data.Substring(offset);
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
