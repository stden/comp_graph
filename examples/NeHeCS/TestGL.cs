using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;
using NeHeCS;
using OpenGL;

namespace MyFormProject 
{
	/// <summary>
	/// Example implementation of the BaseGLControl
	/// </summary>
	public class TestGL : BaseGLControl
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		bool direction = false;
		public TestGL(bool direction)
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			this.direction  = direction;
			this.KeyPress += new KeyPressEventHandler(TestGL_KeyPress);
		}

		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
		}
		#endregion

		long lastMs = 0;
		float angle = 0;
		/// <summary>
		/// Override OnPaint to draw our gl scene
		/// </summary>
		protected override void OnPaint( System.Windows.Forms.PaintEventArgs e )
		{
			float rot1 = 0,rot2 = 0;

			if(DC == 0 || RC == 0)
				return;

			if(lastMs == 0)
				lastMs = DateTime.Now.Ticks;
			long currentMs = DateTime.Now.Ticks;
			long milliseconds = (currentMs - lastMs) / 10000;
			lastMs = currentMs;										//Calculate elapsed timer

			WGL.wglMakeCurrent(DC,RC);

			GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
			GL.glLoadIdentity();

			GL.glTranslatef (0.0f, 0.0f, -6.0f);						// Translate 6 Units Into The Screen
			GL.glRotatef (angle, 0.0f, 1.0f, 0.0f);						// Rotate On The Y-Axis By angle

			for (rot1=0; rot1<2; rot1++)								// 2 Passes
			{
				GL.glRotatef(90.0f,0.0f,1.0f,0.0f);						// Rotate 90 Degrees On The Y-Axis
				GL.glRotatef(180.0f,1.0f,0.0f,0.0f);					// Rotate 180 Degress On The X-Axis
				for (rot2=0; rot2<2; rot2++)							// 2 Passes
				{
					GL.glRotatef(180.0f,0.0f,1.0f,0.0f);				// Rotate 180 Degrees On The Y-Axis
					GL.glBegin (GL.GL_TRIANGLES);						// Begin Drawing Triangles
					GL.glColor3f (1.0f, 0.0f, 0.0f);	GL.glVertex3f( 0.0f, 1.0f, 0.0f);
					GL.glColor3f (0.0f, 1.0f, 0.0f);	GL.glVertex3f(-1.0f,-1.0f, 1.0f);
					GL.glColor3f (0.0f, 0.0f, 1.0f);	GL.glVertex3f( 1.0f,-1.0f, 1.0f);
					GL.glEnd ();										// Done Drawing Triangles
				}
			}

			GL.glFlush ();													// Flush The GL Rendering Pipeline
			WGL.wglSwapBuffers(DC);
			angle += (float)(milliseconds) / 5.0f;	
		}

		/// <summary>
		/// Handle keys, specifically escape
		/// </summary>
		private void TestGL_KeyPress(object sender, KeyPressEventArgs e)
		{
			if(e.KeyChar == (char)Keys.Escape)
				MainForm.App.Close();
		}
	}
}
