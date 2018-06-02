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

    var lastX = 0f
    var lastY = 0f
    var last1X = 0f
    var last1Y = 0f
    var canRotate = true
    var canScale = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_cube_texture)
        controler = NativeOpenglControler()
        this.window.setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        var b = BitmapFactory.decodeResource(resources, R.mipmap.saber)
        cube.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
                controler?.resetSize(width, height, holder?.surface)
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                Log.d("tedu", "surfacedestory")

            }

            override fun surfaceCreated(holder: SurfaceHolder?) {
                controler?.initCube(assets, b)
            }
        })

        cube.setOnTouchListener { v, event ->


            when (event.actionMasked) {
                MotionEvent.ACTION_DOWN -> {
                    lastX = event.getX()
                    lastY = event.getY()
                    canRotate = true
                    canScale = false

                }
                MotionEvent.ACTION_POINTER_DOWN -> {
                    canRotate = false
                    canScale = true
                    if (event.pointerCount > 2) {
                        canScale = false
                        return@setOnTouchListener false
                    }
                    val index0 = event.getPointerId(0)
                    val index1 = event.getPointerId(1)
                    lastX = event.getX(index0)
                    lastY = event.getY(index0)
                    last1X = event.getX(index1)
                    last1Y = event.getY(index1)
                    controler.rotate(0.0f, 0.0f, 0.0f)

                }
                MotionEvent.ACTION_POINTER_UP -> {
                    canScale=false
                    canRotate=false
                    controler.rotate(0.0f, 0.0f, 0.0f)

                }
                MotionEvent.ACTION_MOVE -> {
                    if (canRotate) {
                        if(event.pointerCount>=2){
                            controler.rotate(0.0f, 0.0f, 0.0f)
                            return@setOnTouchListener true
                        }
                        val curY = event.y
                        val curX = event.x
                        val vecX = curX - lastX
                        val vecY = curY - lastY
                        lastY = curY
                        lastX = curX
                        val distance = Math.sqrt((vecX * vecX).toDouble() + (vecY * vecY).toDouble())
                        controler.rotate(vecX, vecY, distance.toFloat())
                    } else if (canScale) {
                        val index0 = event.getPointerId(0)
                        val index1 = event.getPointerId(1)
                        val curX = event.getX(index0)
                        val curY = event.getY(index0)
                        val cur1X = event.getX(index1)
                        val cur1Y = event.getY(index1)
                        val lastDis=Math.sqrt(Math.pow((lastX-last1X).toDouble(), 2.0)+Math.pow((lastY-last1Y).toDouble(), 2.0))
                        val curDis=Math.sqrt(Math.pow((curX-cur1X).toDouble(), 2.0)+Math.pow((curY-cur1Y).toDouble(), 2.0))
                        controler.scale((curDis/lastDis).toFloat())
                    }
                }
            }
            return@setOnTouchListener true;
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        controler.stop()
    }
}
