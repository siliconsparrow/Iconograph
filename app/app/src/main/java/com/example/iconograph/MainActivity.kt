package com.example.iconograph

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.os.Bundle
import android.provider.MediaStore
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity


//class MainActivity : AppCompatActivity(), OnTouchListener, CvCameraViewListener2
class MainActivity : AppCompatActivity()
{
    private final val MY_CAMERA_PERMISSION_CODE = 44
    private val CAMERA_REQUEST = 1888
    var theImage: Bitmap? = null
    var photo: String? = null

   //private lateinit var mOpenCvCameraView: CameraBridgeViewBase
   //private var mRgba: Mat? = null

//    private val mLoaderCallback: BaseLoaderCallback = object : BaseLoaderCallback(this) {
//        override fun onManagerConnected(status: Int) {
//            when (status) {
//                SUCCESS -> {
//                    mOpenCvCameraView.enableView()
//                    mOpenCvCameraView.setOnTouchListener(this@MainActivity)
//                }
//                else -> {
//                    super.onManagerConnected(status)
//                }
//            }
//        }
//    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        var mBtnPhoto = findViewById<Button>(R.id.buttonPhoto)
        mBtnPhoto.setOnClickListener {
            if (checkSelfPermission(Manifest.permission.CAMERA) !== PackageManager.PERMISSION_GRANTED) {
                requestPermissions(arrayOf(Manifest.permission.CAMERA), MY_CAMERA_PERMISSION_CODE)
            } else {
                val cameraIntent = Intent(MediaStore.ACTION_IMAGE_CAPTURE)
                startActivityForResult(cameraIntent, CAMERA_REQUEST)
            }
        }

//        mOpenCvCameraView = findViewById<CameraBridgeViewBase>(R.id.color_blob_detection_activity_surface_view);
//        mOpenCvCameraView.visibility = SurfaceView.VISIBLE
//        mOpenCvCameraView.setCvCameraViewListener(this);
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == CAMERA_REQUEST) {
            val photo: Bitmap = data?.extras?.get("data") as Bitmap
            //imageView.setImageBitmap(photo)
        }
    }

//    private fun getEncodedString(bitmap: Bitmap): String? {
//        val os = ByteArrayOutputStream()
//        bitmap.compress(Bitmap.CompressFormat.JPEG, 100, os)
//
//        /* or use below if you want 32 bit images
//
//       bitmap.compress(Bitmap.CompressFormat.PNG, (0–100 compression), os);*/
//        val imageArr: ByteArray = os.toByteArray()
//        return Base64.encodeToString(imageArr, Base64.URL_SAFE)
//    }

//    override fun onDestroy() {
//        super.onDestroy()
//        if (mOpenCvCameraView != null)
//            mOpenCvCameraView.disableView()
//    }
//    override fun onPause() {
//        super.onPause()
//        if(mOpenCvCameraView != null)
//            mOpenCvCameraView.disableView()
//    }
//
//    override fun onResume() {
//        super.onResume()
//        if (!OpenCVLoader.initDebug()) {
//            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback)
//        } else {
//            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS)
//        }
//    }
//
//    override fun onCameraViewStarted(width: Int, height: Int) {
//        mRgba = Mat(height, width, CvType.CV_8UC4)
//        //mDetector = ColorBlobDetector()
//        //mSpectrum = Mat()
//        //mBlobColorRgba = Scalar(255)
//        //mBlobColorHsv = Scalar(255)
//        //SPECTRUM_SIZE = Size(200, 64)
//        //CONTOUR_COLOR = Scalar(255, 0, 0, 255)
//    }
//
//    override fun onCameraViewStopped() {
//        mRgba!!.release()
//    }
//
//    override fun onCameraFrame(inputFrame: CvCameraViewFrame): Mat? {
//        mRgba = inputFrame.rgba()
//        //if (mIsColorSelected) {
//        //    mDetector.process(mRgba)
//        //    val contours: List<MatOfPoint> = mDetector.getContours()
//        //    Log.e(TAG, "Contours count: " + contours.size)
//        //    Imgproc.drawContours(mRgba, contours, -1, CONTOUR_COLOR)
//        //    val colorLabel = mRgba.submat(4, 68, 4, 68)
//        //    colorLabel.setTo(mBlobColorRgba)
//        //    val spectrumLabel = mRgba.submat(4, 4 + mSpectrum.rows(), 70, 70 + mSpectrum.cols())
//        //    mSpectrum.copyTo(spectrumLabel)
//        //}
//        return mRgba
//    }

//    override fun onTouch(v: View?, event: MotionEvent): Boolean {
//        val cols = mRgba!!.cols()
//        val rows = mRgba!!.rows()
//        val xOffset = (mOpenCvCameraView.width - cols) / 2
//        val yOffset = (mOpenCvCameraView.height - rows) / 2
//        val x = event.x.toInt() - xOffset
//        val y = event.y.toInt() - yOffset
//        Log.i(TAG, "Touch image coordinates: ($x, $y)")
//        if (x < 0 || y < 0 || x > cols || y > rows) return false
//        val touchedRect = Rect()
//        touchedRect.x = if (x > 4) x - 4 else 0
//        touchedRect.y = if (y > 4) y - 4 else 0
//        touchedRect.width = if (x + 4 < cols) x + 4 - touchedRect.x else cols - touchedRect.x
//        touchedRect.height = if (y + 4 < rows) y + 4 - touchedRect.y else rows - touchedRect.y
//        val touchedRegionRgba = mRgba!!.submat(touchedRect)
//        val touchedRegionHsv = Mat()
//        Imgproc.cvtColor(touchedRegionRgba, touchedRegionHsv, Imgproc.COLOR_RGB2HSV_FULL)
//
//        // Calculate average color of touched region
//        mBlobColorHsv = Core.sumElems(touchedRegionHsv)
//        val pointCount: Int = touchedRect.width * touchedRect.height
//        for (i in 0 until mBlobColorHsv.`val`.length) mBlobColorHsv.`val`.get(i) /= pointCount
//        mBlobColorRgba = converScalarHsv2Rgba(mBlobColorHsv)
//        Log.i(
//            TAG,
//            "Touched rgba color: (" + mBlobColorRgba.`val`.get(0)
//                .toString() + ", " + mBlobColorRgba.`val`.get(1).toString() +
//                    ", " + mBlobColorRgba.`val`.get(2).toString() + ", " + mBlobColorRgba.`val`.get(
//                3
//            ).toString() + ")"
//        )
//        mDetector.setHsvColor(mBlobColorHsv)
//        Imgproc.resize(
//            mDetector.getSpectrum(),
//            mSpectrum,
//            SPECTRUM_SIZE,
//            0.0,
//            0.0,
//            Imgproc.INTER_LINEAR_EXACT
//        )
//        mIsColorSelected = true
//        touchedRegionRgba.release()
//        touchedRegionHsv.release()
//        return false // don't need subsequent touch events
//    }
}
