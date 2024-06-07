using System;
using System.Net.Sockets;
using System.Text;
using Gtk;

namespace DotNetApp
{
    class Program
    {
        static void Main(string[] args)
        {
            Application.Init();

            // Create a new window
            Window window = new Window("Calculator");
            window.SetDefaultSize(300, 200);
            window.SetPosition(WindowPosition.Center);
            window.DeleteEvent += (o, e) => Application.Quit();

            // Create a vertical box to hold all widgets
            VBox vbox = new VBox();

            // Create an entry widget for displaying input and results
            Entry entry = new Entry();
            vbox.PackStart(entry, false, false, 5);

            // Add KeyPress event handler to entry
            entry.KeyPressEvent += (o, args) =>
            {
                if (args.Event.Key == Gdk.Key.Return || args.Event.Key == Gdk.Key.KP_Enter)
                {
                    string expression = entry.Text;
                    string result = Calculate(expression);
                    entry.Text = result;
                }
            };

            // Create buttons for digits and operations
            string[] buttons = { "7", "8", "9", "/", "4", "5", "6", "*", "1", "2", "3", "-", "0", "=", "+", "C" };
            Table table = new Table(4, 4, true);

            for (int i = 0; i < buttons.Length; i++)
            {
                string label = buttons[i];
                Button button = new Button(label);
                button.Clicked += (sender, e) => OnButtonClicked(entry, label);
                table.Attach(button, (uint)(i % 4), (uint)(i % 4 + 1), (uint)(i / 4), (uint)(i / 4 + 1));
            }

            vbox.PackStart(table, true, true, 5);
            window.Add(vbox);
            window.ShowAll();
            Application.Run();
        }

        static void OnButtonClicked(Entry entry, string label)
        {
            if (label == "C")
            {
                entry.Text = "";
            }
            else if (label == "=")
            {
                string expression = entry.Text;
                string result = Calculate(expression);
                entry.Text = result;
            }
            else
            {
                entry.Text += label;
            }
        }

        static string Calculate(string expression)
        {
            try
            {
                // Connect to the server
                using (TcpClient client = new TcpClient("127.0.0.1", 65432))
                using (NetworkStream stream = client.GetStream())
                {
                    // Send the expression to the server
                    byte[] data = Encoding.ASCII.GetBytes(expression + "\n");
                    stream.Write(data, 0, data.Length);

                    // Receive the result from the server
                    data = new byte[1024];
                    int bytes = stream.Read(data, 0, data.Length);
                    return Encoding.ASCII.GetString(data, 0, bytes).Trim();
                }
            }
            catch (Exception ex)
            {
                return $"Error: {ex.Message}";
            }
        }
    }
}
