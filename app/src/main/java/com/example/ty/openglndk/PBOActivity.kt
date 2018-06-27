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
        op.inPreferredConfig=Bitmap.Config.ARGB_8888
        var a = BitmapFactory.decodeResource(resources, R.mipmap.bigpic, op)
        var b = BitmapFactory.decodeResource(resources, R.mipmap.bigpic2, op)
//        var b = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_2, op)
//        var c = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_3, op)
//        var e = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_5, op)
//        var f = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_6, op)
//        var g = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_7, op)
//        var h = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_8, op)
//        var i = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_9, op)
//        var j = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_10, op)
//        var k = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_11, op)
//        var l = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_12, op)
//        var m = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_13, op)
//        var n = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_14, op)
//        var o = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_15, op)
//        var p = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_16, op)
//        var q = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_17, op)
//        var r = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_18, op)
//        var s = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_19, op)
//        var t = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_20, op)
//        var t2 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_21, op)
//        var t3 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_22, op)
//        var t4 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_23, op)
//        var t5 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_24, op)
//        var t6 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_25, op)
//        var t7 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_26, op)
//        var t8 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_27, op)
//        var t9 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_28, op)
//        var t11 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_29, op)
//        var t12 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_30, op)
//        var t13 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_31, op)
//        var t14 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_32, op)
//        var t15 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_33, op)
//        var t16 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_34, op)
//        var t17 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_35, op)
//        var t18 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_36, op)
//        var t19 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_37, op)
//        var t20 = BitmapFactory.decodeResource(resources, R.mipmap.xiaoren_38, op)
        val bitmapArray = arrayOf<Bitmap>(
                a,b
//                b,
//                c,
//                e,
//                f,
//                g,
//                h,
//                i,
//                j,
//                k,
//                l,
//                m,
//                n,
//                o,
//                p,
//                q,
//                r,
//                s
//                t,
//                t2,
//                t3,
//                t4,
//                t5,
//                t6,
//                t7,
//                t8,
//                t9,
//                t11,
//                t12,
//                t13,
//                t14,
//                t15,
//                t16,
//                t17,
//                t18,
//                t19,
//                t20
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
