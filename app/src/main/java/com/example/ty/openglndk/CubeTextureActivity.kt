package com.example.ty.openglndk

import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.SurfaceHolder
import android.view.WindowManager
import kotlinx.android.synthetic.main.activity_cube_texture.*

class CubeTextureActivity : AppCompatActivity() {

    private lateinit var controler: NativeOpenglControler

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_cube_texture)
        controler = NativeOpenglControler()
        this.window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        var b=BitmapFactory.decodeResource(resources,R.mipmap.saber)
        cube.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
                controler?.resetSize(width, height,holder?.surface)
            }
            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                Log.d("tedu", "surfacedestory")

            }
            override fun surfaceCreated(holder: SurfaceHolder?) {
                controler?.initCube(assets,b)
            }
        })

    }

    override fun onDestroy() {
        super.onDestroy()
        controler?.stop()
    }
}
