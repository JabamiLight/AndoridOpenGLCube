package com.example.ty.openglndk

import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import org.jetbrains.anko.startActivity

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        btn_vao.setOnClickListener {
            startActivity<VAOVBOActivity>();
        }
        btn_cube_texture.setOnClickListener {
            startActivity<CubeTextureActivity>()
        }
    }
}
