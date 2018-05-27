package com.example.ty.openglndk

import android.content.Context
import android.graphics.Canvas
import android.util.AttributeSet
import android.view.View
import android.view.animation.AlphaAnimation
import android.view.animation.Animation
import android.view.animation.LinearInterpolator
import android.view.animation.Transformation

/*
* Created by TY on 2018/5/26.
*      
*
*      
*          ┌─┐       ┌─┐
*       ┌──┘ ┴───────┘ ┴──┐
*       │                 │
*       │       ───       │
*       │  ─┬┘       └┬─  │
*       │                 │
*       │       ─┴─       │
*       │                 │
*       └───┐         ┌───┘
*           │         │
*           │         │
*           │         │
*           │         └──────────────┐
*           │                        │
*           │                        ├─┐
*           │                        ┌─┘    
*           │                        │
*           └─┐  ┐  ┌───────┬──┐  ┌──┘         
*             │ ─┤ ─┤       │ ─┤ ─┤         
*             └──┴──┘       └──┴──┘ 
*                 神兽保佑 
*                 代码无BUG! 
*/
class TestView(context: Context?, attrs: AttributeSet?) : View(context, attrs) {

    override fun onDraw(canvas: Canvas?) {
        super.onDraw(canvas)
//        Log.d("teud", "$drawingTime")
//        Log.e("tedu", "${sd?.getTransformation(drawingTime, trans)}")
//        Log.v("tedu","${trans.alpha}-----")
//        postInvalidate()
    }

    private var sd: AlphaAnimation? = null
    val trans = Transformation()

    override fun onAttachedToWindow() {
        super.onAttachedToWindow()
        sd = AlphaAnimation(0f, 1f);
        sd?.duration = 3000
        sd?.repeatMode = AlphaAnimation.RESTART
        sd?.startTime = -1
        sd?.repeatCount = Animation.INFINITE
        sd?.interpolator = LinearInterpolator()
        sd?.setAnimationListener(object :Animation.AnimationListener{
            override fun onAnimationRepeat(animation: Animation?) {
            }

            override fun onAnimationEnd(animation: Animation?) {
            }

            override fun onAnimationStart(animation: Animation?) {
            }

        })
    }
}