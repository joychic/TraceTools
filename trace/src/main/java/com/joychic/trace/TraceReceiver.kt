package com.joychic.trace

import android.annotation.SuppressLint
import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.os.Build
import android.util.Log

/**
 * @Date: 2024/12/5 13:44
 * @author: chi.jiang
 * @Description:
 * open:  adb shell am broadcast -a TRACE.ACTION_START -f 0x01000000
 * close: adb shell am broadcast -a TRACE.ACTION_STOP -f 0x01000000
 * update prop: adb shell am broadcast -a TRACE.ACTION_OPEN -f 0x01000000
 **/
class TraceReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context?, intent: Intent?) {
        Log.d(TAG, "onReceive: $intent")
        intent?.apply {
            when (action) {
                "TRACE.ACTION_START" -> {
                    MTrace.start()
                }

                "TRACE.ACTION_STOP" -> {
                    MTrace.stop()
                    if (SystemPropertiesUtils.getBoolean(TRACE, false)) {
                        SystemPropertiesUtils.set(TRACE, "false")
                    }
                }

                "TRACE.ACTION_OPEN" -> {
                    SystemPropertiesUtils.set(TRACE, "true")
                }
            }
        }
    }

    companion object {
        private const val TAG = "TraceReceiver"
        private const val TRACE = "TRACE"
        private const val ACTION_START = "TRACE.ACTION_START"
        private const val ACTION_STOP = "TRACE.ACTION_STOP"
        private const val ACTION_OPEN = "TRACE.ACTION_OPEN"

        @JvmStatic
        @SuppressLint("UnspecifiedRegisterReceiverFlag")
        fun initTraceReceiver(context: Context) {
            val mReceiver = TraceReceiver()
            val filter = IntentFilter()
            filter.addAction(ACTION_START)
            filter.addAction(ACTION_STOP)
            filter.addAction(ACTION_OPEN)
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
                context.registerReceiver(mReceiver, filter, Context.RECEIVER_EXPORTED)
            } else {
                context.registerReceiver(mReceiver, filter)
            }
        }

        fun isTraceOpen() = SystemPropertiesUtils.getBoolean(TRACE, false)
    }
}