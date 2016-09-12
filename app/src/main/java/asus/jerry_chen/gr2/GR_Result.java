package asus.jerry_chen.gr2;

/**
 * Created by Jerry_Chen on 2016/9/12.
 */
public class GR_Result {

    static {
        System.loadLibrary("gr");
    }

    //  public native String getStringFromJni();

    public native int getResultFromJni(String string);
}
