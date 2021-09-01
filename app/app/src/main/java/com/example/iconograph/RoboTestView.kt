package com.example.iconograph
import android.content.Context
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Paint
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.View
import androidx.core.content.res.ResourcesCompat
import org.opencv.core.Point

class RoboTestView(context: Context, attrs: AttributeSet?) : View(context, attrs) {

    private lateinit var extraCanvas: Canvas
    private lateinit var extraBitmap: Bitmap
    private val backgroundColor = ResourcesCompat.getColor(resources, R.color.colorBackground, null)

    private val drawColor = ResourcesCompat.getColor(resources, R.color.colorPaint, null)
    private val paint = Paint().apply {
        color = drawColor
        isAntiAlias = true
        strokeWidth = 10f
    }

    private var touchX: Float = 0f
    private var touchY: Float = 0f

    private var shape: List<Point> = emptyList()

    fun setShape(pt: List<Point>) {
        shape = pt
        invalidate()
    }

    override fun onSizeChanged(width: Int, height: Int, oldWidth: Int, oldHeight: Int) {
        super.onSizeChanged(width, height, oldWidth, oldHeight)
        extraBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        extraCanvas = Canvas(extraBitmap)
        extraCanvas.drawColor(backgroundColor)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        canvas.drawBitmap(extraBitmap, 0f, 0f, null)
        if(shape.isEmpty()) {
            canvas.drawCircle(touchX, touchY, 100f, paint)
        } else {
            var lastX = (shape[0].x.toFloat() * width) / 65535
            var lastY = (shape[0].y.toFloat() * height) / 65535
            for(p in shape) {
                var x = (p.x.toFloat() * width) / 65535
                var y = (p.y.toFloat() * height) / 65535
                canvas.drawLine(lastX, lastY, x, y, paint)
                lastX = x
                lastY = y
            }
        }
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {

        if(event.action == MotionEvent.ACTION_DOWN || event.action == MotionEvent.ACTION_MOVE) {
            touchX = event.x
            touchY = event.y
            shape = emptyList()
            invalidate()
        }
        return true
    }
}
