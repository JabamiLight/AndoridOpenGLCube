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
        var a = BitmapFactory.decodeResource(resources, R.mipmap.pic1)
        var b = BitmapFactory.decodeResource(resources, R.mipmap.pic2)
        var c = BitmapFactory.decodeResource(resources, R.mipmap.pic3)
        var d = BitmapFactory.decodeResource(resources, R.mipmap.pic4)
        var e = BitmapFactory.decodeResource(resources, R.mipmap.pic5)
        var f = BitmapFactory.decodeResource(resources, R.mipmap.pic6)
        var g = BitmapFactory.decodeResource(resources, R.mipmap.pic7)
        val bitmapArray= arrayOf<Bitmap>(a,b,c,d,e,f,g)
        surface.holder.addCallback(object: SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
                controler?.resetSize(width, height, holder?.surface)
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                controler?.stop()
            }

            override fun surfaceCreated(holder: SurfaceHolder?) {
                controler?.initPbo(assets,bitmapArray)
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
        controler?.stop()
    }

}
