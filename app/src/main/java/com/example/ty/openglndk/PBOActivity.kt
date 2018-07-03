package com.example.ty.openglndk

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.SurfaceHolder
import kotlinx.android.synthetic.main.activity_pbo.*

class PBOActivity : AppCompatActivity() {

    private var controler: NativeOpenglControler? = null
    private lateinit var intArray: Array<Int>


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_pbo)
        controler = NativeOpenglControler()
        controler?.frameRateListener = {
            runOnUiThread {
                tv_frame.text = it.toString()
            }
        }
        val op = BitmapFactory.Options()
        var a = BitmapFactory.decodeResource(resources, R.mipmap.pic5, op)
//        var b = BitmapFactory.decodeResource(resources, R.mipmap.bigpic2, op)
        val bitmapArray = arrayOf<Bitmap>(
                a
        )
        surface.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
                controler?.resetSize(width, height, holder?.surface)
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                controler?.stop()
            }

            override fun surfaceCreated(holder: SurfaceHolder?) {
                controler?.initPbo(assets, bitmapArray)
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
        controler?.stop()
    }

}
