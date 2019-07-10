using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

namespace MazeSockitToMe
{
    class Program
    {
        public static void StartClient()
        {
            var buff = new byte[8196];
            const string IP = "192.168.5.22";
            const int Port = 4242;
            var ipAddress = IPAddress.Parse(IP);

            IPEndPoint remoteEndPoint = new IPEndPoint(ipAddress, Port);

            var sender = new Socket(ipAddress.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

            try
            {
                sender.Connect(remoteEndPoint);
                Console.WriteLine("Socket connected to {0}", sender.RemoteEndPoint.ToString());

                int bytesRec = sender.Receive(buff);
                Console.WriteLine("Received: {0}", Encoding.ASCII.GetString(buff, 0, bytesRec));

                sender.Shutdown(SocketShutdown.Both);
                sender.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
        }

        static void Main(string[] args)
        {
            StartClient();
        }
    }
}
