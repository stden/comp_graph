using System;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using MyFormProject;

namespace NeHeCS
{
    /// <summary>
    ///     Example form to contain the example implementation of BaseGLControl
    /// </summary>
    internal class MainForm : Form
    {
        private static Form _this;
        private readonly TestGL glControl = new TestGL(true); //Example implementation
        private Timer updateTimer = new Timer(); //Refresh  timer

        public MainForm()
        {
            InitializeComponent();
            glControl.Location = new Point(0, 0); //Position control at 0
            glControl.Dock = DockStyle.Fill; //Dock to fill form
            glControl.Visible = true;


            Load += MainForm_Load; //Add load handler to create timer
            Closing += MainForm_Closing;
            Controls.Add(glControl);
        }

        /// <summary>
        ///     Singleton for accessing our application
        /// </summary>
        public static Form App
        {
            get { return _this ?? (_this = new MainForm()); }
        }

        private void InitializeComponent()
        {
            // 
            // MainForm
            // 
            AutoScaleBaseSize = new Size(5, 13);
            ClientSize = new Size(292, 273);
            Name = "MainForm";
            Text = "NeHe C# Framework";
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (updateTimer != null) //Close refresh timer
                {
                    updateTimer.Stop();
                    updateTimer.Dispose();
                    updateTimer = null;
                }
            }
            base.Dispose(disposing);
        }

        /// <summary>
        ///     When the form loads create a refresh timer
        /// </summary>
        private void MainForm_Load(object sender, EventArgs e)
        {
            updateTimer.Interval = 10;
            updateTimer.Tick += updateTimer_Tick;
            updateTimer.Start();
        }

        /// <summary>
        ///     When the timer fires, refresh control
        /// </summary>
        private void updateTimer_Tick(object sender, EventArgs e)
        {
            glControl.Invalidate();
        }

        /// <summary>
        ///     When the form closes, close the refresh timer
        /// </summary>
        private void MainForm_Closing(object sender, CancelEventArgs e)
        {
            if (updateTimer != null)
            {
                updateTimer.Stop();
                updateTimer.Dispose();
                updateTimer = null;
            }
        }

        [STAThread]
        public static void Main(string[] args)
        {
            DialogResult res = MessageBox.Show(null, "Would You Like To Run In Fullscreen Mode?",
                "Start Fullscreen?", MessageBoxButtons.YesNo, MessageBoxIcon.Information);
            var form = (MainForm) App;
            if (res == DialogResult.Yes)
            {
                form.FormBorderStyle = FormBorderStyle.None;
                form.Location = new Point(0, 0);
                form.Size = Screen.PrimaryScreen.Bounds.Size;
            }
            Application.Run(form);
        }
    }
}