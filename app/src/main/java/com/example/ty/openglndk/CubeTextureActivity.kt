package com.example.ty.openglndk

import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.WindowManager
import kotlinx.android.synthetic.main.activity_cube_texture.*

class CubeTextureActivity : AppCompatActivity() {

    private lateinit var controler: NativeOpenglControler

    private var lastX=0f;
    private var lastY=0f;

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

        cube.setOnTouchListener { v, event ->

            when(event.action){
                MotionEvent.ACTION_DOWN->{
                    lastX=event.getX()
                    lastY=event.getY()
                }
                MotionEvent.ACTION_MOVE->{
                    var curY=event.y
                    var curX=event.x

                    var vecX=event.x-lastX
                    var vecY=event.y-lastY

                    var distance=Math.sqrt((vecX*vecX).toDouble()+(vecY*vecY).toDouble())
                    controler.rotate(vecX,vecY,distance.toFloat())
                }
            }



            return@setOnTouchListener true;
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        controler?.stop()
    }
}
