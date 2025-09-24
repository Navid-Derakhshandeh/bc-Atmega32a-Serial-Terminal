using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;

namespace Serial_Terminal
{
    public partial class Form1 : Form
    {
        SerialPort serialPort = new SerialPort();
        StringBuilder receiveBuffer = new StringBuilder();

        public Form1()
        {
            InitializeComponent();
            comboBoxPorts.Items.AddRange(SerialPort.GetPortNames());
            serialPort.DataReceived += SerialPort_DataReceived;
        }

        private void buttonConnect_Click(object sender, EventArgs e)
        {

            if (!serialPort.IsOpen)
            {
                try
                {
                    serialPort.PortName = comboBoxPorts.SelectedItem.ToString();
                    serialPort.BaudRate = 2400; // Match AVR baud rate
                    serialPort.DataBits = 8;
                    serialPort.Parity = Parity.None;
                    serialPort.StopBits = StopBits.One;
                    serialPort.Handshake = Handshake.None;
                    serialPort.DtrEnable = true;
                    serialPort.RtsEnable = true;
                    serialPort.NewLine = "\r\n"; // Match AVR line endings
                    serialPort.Open();
                    Log("Connected to " + serialPort.PortName);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error: " + ex.Message);
                }
            }
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (serialPort.IsOpen)
            {
                string message = textBoxSend.Text + "\r"; // Send carriage return
                serialPort.Write(message);
                Log("Sent: " + textBoxSend.Text);
                textBoxSend.Clear();
            }
        }
        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data = serialPort.ReadExisting();
            receiveBuffer.Append(data);

            while (receiveBuffer.ToString().Contains("\r\n"))
            {
                string fullText = receiveBuffer.ToString();
                int index = fullText.IndexOf("\r\n");
                string line = fullText.Substring(0, index);
                receiveBuffer.Remove(0, index + 2); // Remove processed line

                Invoke(new Action(() => Log(line))); // Clean line output
            }
        }

        private void Log(string message)
        {
            textBoxLog.AppendText(message + Environment.NewLine);
        }
    }
}
