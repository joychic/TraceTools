# # TraceTools
MTrace is an Android dynamic ATrace tool that utilizes ART method hooks. Work with [Perfetto](https://perfetto.dev/docs/), which allows you to collect system-wide performance traces from Android devices from a variety of data sources (kernel scheduler via ftrace, userspace instrumentation via atrace and all other data sources listed in this site).


### Getting Started
#### Installation Via Maven
Add the following lines to build.gradle on your app module:
```
dependencies {
   implementation 'io.gitee.joychic:TraceTools:0.0.1'
    // replace "0.0.1" with any available version
}
```

### Quick Tutorial

Setup MTrace on App startup, say your Application class, add these lines:

```
    override fun onCreate() {
        super.onCreate()
        MTrace.start(
            MTrace.ConfigBuilder()
                .isDebug(false)
                .setMethodName("com.joychic.trace")
                .build()
        )
    }
```

#### [Download record_android_trace](https://perfetto.dev/docs/quickstart/android-tracing)

On Linux and Mac:
```
curl -O https://raw.githubusercontent.com/google/perfetto/main/tools/record_android_trace
```
then, chmod record_android_trace
> chmod u+x record_android_trace

you can also add alias to your `.bashrc` or `.zshrc`

``` 
alias perfetto='${ANDROID_HOME}/record_android_trace -o trace_file.perfetto-trace -t 20s -b 1024mb  gfx input view webview wm am sm audio video camera dalvik rs bionic sched freq idle disk load sync binder_lock'
```

On Windows:
```
curl -O https://raw.githubusercontent.com/google/perfetto/main/tools/record_android_trace
```


#### Recording a trace
```
 perfetto -a com.joychic.trace
```
replace `com.joychic.trace` with your application

when finished, press `Control+C`    `https://ui.perfetto.dev/#!/record/cpu`

### Performance
![](doc/17231951542551.jpg)