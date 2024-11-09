package com.joychic.trace.ui.main

import android.util.Log
import androidx.lifecycle.ViewModel
import kotlin.concurrent.thread

class MainViewModel : ViewModel() {

    fun click1() {
        Log.d("trace", "1111")
    }

    fun click2() {
        Log.d("trace", "2222")
    }

    fun click3() {
        Log.d("trace", "3333")
    }

    fun click4() {
        thread {
            Log.d("trace", "4444    ${Thread.currentThread().name}")
        }
    }
}