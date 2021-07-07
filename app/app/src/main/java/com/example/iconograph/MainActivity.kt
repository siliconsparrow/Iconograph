package com.example.iconograph

import android.Manifest
import android.app.Activity
import android.app.AlertDialog
import android.bluetooth.*
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.os.Build
import android.os.Bundle
import android.provider.MediaStore
import android.util.Log
import android.widget.Button
import android.widget.SeekBar
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat

// TODO: Move camera and image stuff to separate classes.
// TODO: Interactive camera image
// TODO: Interactive contour display and parameter controls
// TODO: Convert to vectors
// TODO: Send vectors to robot
// TODO: Receive progress updates from robot

//class MainActivity : AppCompatActivity(), OnTouchListener, CvCameraViewListener2
class MainActivity : AppCompatActivity()
{
    private val MY_CAMERA_PERMISSION_CODE = 44
    private val CAMERA_REQUEST = 1888
    private val LOCATION_PERMISSION_REQUEST_CODE = 2
    private val ENABLE_BLUETOOTH_REQUEST_CODE = 1

    private val isLocationPermissionGranted get() = hasPermission(Manifest.permission.ACCESS_FINE_LOCATION)

    var theImage: Bitmap? = null
    var photo: String? = null

    // Set up the BLE Communications object. Lazy init after the MainActivity has been constructed.
    private val comm: BleComm by lazy {
        BleComm(this)
    }

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

        findViewById<SeekBar>(R.id.seekBar1).setOnSeekBarChangeListener(object:SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(seekBar: SeekBar, i: Int, b:Boolean) {
                testSetServo(1, i)
            }
            override fun onStartTrackingTouch(seekBar: SeekBar) { }
            override fun onStopTrackingTouch(seekBar: SeekBar) { }
        })

        findViewById<SeekBar>(R.id.seekBar2).setOnSeekBarChangeListener(object:SeekBar.OnSeekBarChangeListener{
            override fun onProgressChanged(seekBar: SeekBar, i: Int, b:Boolean) {
                testSetServo(2, i)
            }
            override fun onStartTrackingTouch(seekBar: SeekBar) { }
            override fun onStopTrackingTouch(seekBar: SeekBar) { }
        })

        val mBtnPhoto = findViewById<Button>(R.id.buttonPhoto)
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

    private fun testSetServo(id: Int, pos:Int) {
        if(comm.isConnected()) {
            var pkt = ByteArray(3)
            pkt[0] = 0x02
            pkt[1] = id.toByte()
            pkt[2] = pos.toByte()
            comm.send(pkt)
        }
    }

    // Update the status text box with the given string.
    // TODO: Could it take a enunmerated type instead?
    fun setStatusText(tx: String) {
        val txStatus = findViewById<TextView>(R.id.textViewStatus)
        txStatus.text = tx
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if(requestCode == ENABLE_BLUETOOTH_REQUEST_CODE) {
            if (resultCode != Activity.RESULT_OK) {
                promptEnableBluetooth()
            } else {
                comm.resume()
            }
        }

        if(requestCode == CAMERA_REQUEST) {
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

    override fun onDestroy() {
        super.onDestroy()
        comm.destroy()
//        if (mOpenCvCameraView != null)
//            mOpenCvCameraView.disableView()
    }

    override fun onPause() {
        super.onPause()
        comm.pause()
//        if(mOpenCvCameraView != null)
//            mOpenCvCameraView.disableView()
    }

    override fun onResume() {
        super.onResume()

        // Check Bluetooth is turned on.
        if (!comm.isBluetoothEnabled()) {
            promptEnableBluetooth()

        // Next check if we have location permissions (required to use BLE scanner)
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !isLocationPermissionGranted) {
            requestLocationPermission()

        // If we have all that, we can start scanning for our device.
        } else {
            comm.resume()
        }

//        if (!OpenCVLoader.initDebug()) {
//            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback)
//        } else {
//            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS)
//        }
    }

    // Check if Bluetooth is enabled and if not, ask the user if we can enable it.
    private fun promptEnableBluetooth() {
        if (!comm.isBluetoothEnabled()) {
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(enableBtIntent, ENABLE_BLUETOOTH_REQUEST_CODE)
        }
    }

    fun Context.hasPermission(permissionType: String): Boolean {
        return ContextCompat.checkSelfPermission(this, permissionType) == PackageManager.PERMISSION_GRANTED
    }

    private fun requestLocationPermission() {
        if (isLocationPermissionGranted) {
            return
        }
        runOnUiThread {
            var dlgBuilder = AlertDialog.Builder(this)
            dlgBuilder.setTitle("Location permission required")
            dlgBuilder.setMessage("Starting from Android M (6.0), the system requires apps to be granted location access in order to scan for BLE devices.")
            dlgBuilder.setIcon(android.R.drawable.ic_dialog_alert)
            dlgBuilder.setPositiveButton(android.R.string.ok){dialogInterface, which->requestPermission(
                Manifest.permission.ACCESS_FINE_LOCATION,
                LOCATION_PERMISSION_REQUEST_CODE
            )}
            var dlg: AlertDialog = dlgBuilder.create()
            dlg.show()
        }
    }

    private fun Activity.requestPermission(permission: String, requestCode: Int) {
        ActivityCompat.requestPermissions(this, arrayOf(permission), requestCode)
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        when (requestCode) {
            LOCATION_PERMISSION_REQUEST_CODE -> {
                if (grantResults.firstOrNull() == PackageManager.PERMISSION_DENIED) {
                    requestLocationPermission()
                } else {
                    comm.resume() //btnBleScan()
                }
            }
        }
    }

//    private fun btnTest() {
//        comm.send("Hello World".toByteArray())
//    }

    fun recvd(str: String) {
        Log.w("APP", "RX: $str")
    }

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
