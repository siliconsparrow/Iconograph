package com.example.iconograph

import android.Manifest
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.Bitmap
import android.os.Bundle
import android.os.Build
import android.provider.MediaStore
import android.widget.Button
import android.content.Context
import android.app.Activity
import android.app.AlertDialog
import android.bluetooth.*
import android.content.DialogInterface
import androidx.core.content.ContextCompat
import androidx.core.app.ActivityCompat
import androidx.appcompat.app.AppCompatActivity
import android.bluetooth.le.ScanCallback
import android.bluetooth.le.ScanFilter
import android.bluetooth.le.ScanResult
import android.bluetooth.le.ScanSettings
import android.graphics.Color
import android.util.Log

private const val ENABLE_BLUETOOTH_REQUEST_CODE = 1

//class MainActivity : AppCompatActivity(), OnTouchListener, CvCameraViewListener2
class MainActivity : AppCompatActivity()
{
    private final val MY_CAMERA_PERMISSION_CODE = 44
    private final val CAMERA_REQUEST = 1888
    private final val LOCATION_PERMISSION_REQUEST_CODE = 2

    // Bluetooth stuff
    private var filters = ArrayList<ScanFilter>()

    private val bluetoothAdapter: BluetoothAdapter by lazy {
        val bluetoothManager = getSystemService(Context.BLUETOOTH_SERVICE) as BluetoothManager
        bluetoothManager.adapter
    }
    private val bleScanner by lazy {
        bluetoothAdapter.bluetoothLeScanner
    }
    private val scanSettings = ScanSettings.Builder()
        .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
        .build()
    private val scanCallback = object : ScanCallback() {
        override fun onScanResult(callbackType: Int, result: ScanResult) {
            with(result.device) {
                scanStop();
                Log.i("BLE", "Found BLE device. Name: ${name ?: "Unnamed"}, address: $address")
                bleConnect(result.device);
            }
        }
    }
    private var isScanning: Boolean = false

    val isLocationPermissionGranted get() = hasPermission(Manifest.permission.ACCESS_FINE_LOCATION)

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

        filters.add(ScanFilter.Builder().setDeviceName("Nordic_UART").build())

        var buttonBleScan = findViewById<Button>(R.id.buttonBleScan)
        buttonBleScan.setOnClickListener{startBleScan()}

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

        when (requestCode) {
            ENABLE_BLUETOOTH_REQUEST_CODE -> {
                if (resultCode != Activity.RESULT_OK) {
                    promptEnableBluetooth()
                }
            }
        }

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

    override fun onDestroy() {
        super.onDestroy()
//        if (mOpenCvCameraView != null)
//            mOpenCvCameraView.disableView()
    }

    override fun onPause() {
        super.onPause()
//        if(mOpenCvCameraView != null)
//            mOpenCvCameraView.disableView()
    }

    override fun onResume() {
        super.onResume()

        if (!bluetoothAdapter.isEnabled) {
            promptEnableBluetooth()
        }

//        if (!OpenCVLoader.initDebug()) {
//            OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this, mLoaderCallback)
//        } else {
//            mLoaderCallback.onManagerConnected(LoaderCallbackInterface.SUCCESS)
//        }
    }

    private fun promptEnableBluetooth() {
        if (!bluetoothAdapter.isEnabled) {
            val enableBtIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(enableBtIntent, ENABLE_BLUETOOTH_REQUEST_CODE)
        }
    }

    fun Context.hasPermission(permissionType: String): Boolean {
        return ContextCompat.checkSelfPermission(this, permissionType) ==
                PackageManager.PERMISSION_GRANTED
    }

    private fun scanStart() {
        Log.w("BLE", "BLE scan start")
        bleScanner.startScan(filters, scanSettings, scanCallback)
        var buttonBleScan = findViewById<Button>(R.id.buttonBleScan);
        buttonBleScan.setText("STOP BLE SCAN")
        buttonBleScan.setBackgroundColor(Color.RED)
        isScanning = true
    }

    private fun scanStop() {
        Log.w("BLE","BLE scan stop")
        bleScanner.stopScan(scanCallback)
        var buttonBleScan = findViewById<Button>(R.id.buttonBleScan);
        buttonBleScan.setText("BLE SCAN")
        buttonBleScan.setBackgroundColor(Color.BLUE)
        isScanning = false
    }

    private val gattCallback = object : BluetoothGattCallback() {
        override fun onConnectionStateChange(gatt: BluetoothGatt, status: Int, newState: Int) {
            val deviceAddress = gatt.device.address
            if(status == BluetoothGatt.GATT_SUCCESS) {
                if(newState == BluetoothProfile.STATE_CONNECTED) {
                    Log.w("BLE", "Connected to $deviceAddress")
                } else {
                    Log.w("BLE", "Disconnected from $deviceAddress")
                    gatt.close()
                }
            } else {
                Log.w("BLE","Error $status with connection to $deviceAddress")
                gatt.close()
            }
        }
    }

    private fun bleConnect(device: BluetoothDevice)  {
        Log.w("ScanResult","Connecting to $device.address")
        device.connectGatt(this, false, gattCallback)
    }

    private fun startBleScan() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !isLocationPermissionGranted) {
            requestLocationPermission()
        }
        else if(isScanning) {
            scanStop()
        } else {
            scanStart()
        }
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
            /*
            dlg.setCancelable(false)
                .setPositiveButton(android.R.string.ok, DialogInterface.OnClickListener{ dialog, id->recreate() })
            var alert = dlg.create()
            alert.setTitle("Location permission required")
            alert.setIcon(R.mipmap.ic_launcher)
            alert.show()


            alert("Location permission required", "Need location permission to scan for BLE devices", android.R.string.ok, )
            alert {
                title = "Location permission required"
                message = "Starting from Android M (6.0), the system requires apps to be granted " +
                        "location access in order to scan for BLE devices."
                isCancelable = false
                positiveButton(android.R.string.ok) {
                    requestPermission(
                        Manifest.permission.ACCESS_FINE_LOCATION,
                        LOCATION_PERMISSION_REQUEST_CODE
                    )
                }
            }.show()*/
        }
    }
/*
    //fun alert(title:String, message:String, isCancelable:Boolean, positiveButton:(String)->Unit): AlertDialog {
    fun alert(): Unit {
        var dlg = AlertDialog.Builder(this)
        var intent = Intent(this, MainActivity::class.java)
        dlg.setMessage(message)
            .setCancelable(isCancelable)
            .setPositiveButton(positiveButton.)
        //.setPositiveButton(positiveButton, DialogInterface.OnClickListener{dialog, id->recreate()})
        var alert = dlg.create()
        alert.setTitle(title)
        alert.setIcon(R.mipmap.ic_launcher)
        return alert // alert.show()
    }
*/
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
                    startBleScan()
                }
            }
        }
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
