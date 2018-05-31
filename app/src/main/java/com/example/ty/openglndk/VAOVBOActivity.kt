package com.example.ty.openglndk

import android.graphics.SurfaceTexture
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.Surface
import android.view.TextureView
import kotlinx.android.synthetic.main.activity_vao_vbo.*


class VAOVBOActivity : AppCompatActivity() {

    private var controler: NativeOpenglControler? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vao_vbo)
        controler = NativeOpenglControler()
        surface.setSurfaceTextureListener(object: TextureView.SurfaceTextureListener {
            override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture?, width: Int, height: Int) {
                controler?.resetSize(width, height, Surface(surface))
            }

            override fun onSurfaceTextureUpdated(surface: SurfaceTexture?) {
            }
            override fun onSurfaceTextureDestroyed(surface: SurfaceTexture?): Boolean {
                controler?.stop()
                return  true;
            }

            override fun onSurfaceTextureAvailable(surface: SurfaceTexture?, width: Int, height: Int) {
                controler?.init(assets)
                controler?.resetSize(width, height,Surface(surface))
            }
        })
    }

    override fun onDestroy() {
        super.onDestroy()
        controler?.stop()
    }


}

