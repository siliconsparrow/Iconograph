package com.example.iconograph

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.DragEvent
import android.view.View
import android.widget.LinearLayout
import android.widget.TextView

class ActivityRoboTest : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_robo_test)

        val mCanvas = RoboTestView(this)
        findViewById<LinearLayout>(R.id.layoutRoboTest).addView(mCanvas)

        var txStatus = findViewById<TextView>(R.id.textViewArmInfo)

        mCanvas.setOnDragListener(object: View.OnDragListener {
            override fun onDrag(v: View, event: DragEvent): Boolean {
                txStatus.setText("{event.x},{event.y}")
                return true
            }
        })
    }
}
