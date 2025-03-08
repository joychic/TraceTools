package com.joychic.trace

import android.annotation.SuppressLint
import android.content.ContentProvider
import android.content.ContentValues
import android.database.Cursor
import android.net.Uri

/**
 * Description:
 *
 * Author: chijiang
 * Date: 2024/11/9
 */
class InitProvider : ContentProvider() {
    @SuppressLint("UnspecifiedRegisterReceiverFlag")
    override fun onCreate(): Boolean {
        context?.apply { TraceReceiver.initTraceReceiver(this) }
        if (TraceReceiver.isTraceOpen()) {
            MTrace.start()
        }
        return true
    }

    override fun query(
        p0: Uri,
        p1: Array<out String>?,
        p2: String?,
        p3: Array<out String>?,
        p4: String?
    ): Cursor? {
        return null
    }

    override fun getType(p0: Uri): String? {
        return null
    }

    override fun insert(p0: Uri, p1: ContentValues?): Uri? {
        return null
    }

    override fun delete(p0: Uri, p1: String?, p2: Array<out String>?): Int {
        return 0
    }

    override fun update(p0: Uri, p1: ContentValues?, p2: String?, p3: Array<out String>?): Int {
        return 0
    }
}