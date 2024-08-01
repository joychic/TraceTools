package com.joychic.trace

import android.app.Application

class App : Application() {
    override fun onCreate() {
        super.onCreate()
        MTrace.start(
            MTrace.ConfigBuilder()
                .isDebug(true)
                .setMethodName("com.joychic.trace")
                .build()
        )
    }
}