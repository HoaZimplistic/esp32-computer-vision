// 21_Color_Blob_Detection_HTTP.ino

/**
 * Run an HTTP server to debug the ColorBlobDetector
 */
#define MAX_RESOLUTION_VGA
#define MAX_RECURSION_DEPTH 13

#include "esp32cam.h"
#include "esp32cam/JpegDecoder.h"
#include "esp32cam/apps/ColorBlobDetector.h"
#include "ColorBlobDetectorHTTP.h"

using namespace Eloquent::Esp32cam;

Cam cam;
JpegDecoder decoder;
Applications::ColorBlobDetector detector(0, 0, 0); // will configure from the URL
Http::ColorBlobDetectorHTTP http(cam, decoder, detector);

// http://esp32cam.local/?y=61&cb=37&cr=28

// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid = "Hoa";
const char* password = "6uifMinhHo@f";

void setup() {
    Serial.begin(115200);
    delay(3000);
    Serial.println("Init");

    cam.aithinker();
    cam.highQuality();
    cam.vga();
    cam.highestSaturation();
    cam.disableAutomaticWhiteBalance();
    cam.disableAutomaticExposureControl();
    cam.disableGainControl();

    /**
     * Set detector tollerance
     * The higher, the more shade of colors it will pick
     */
    detector.tollerate(16);
    /**
     * Skip blob localization (slow) if not enough
     * pixels match color
     */
    detector.setMinArea(30 * 30);

    while (!cam.begin())
        Serial.println(cam.getErrorMessage());

    while (!cam.connect(ssid, password))
        Serial.println(cam.getErrorMessage());

    while (!http.begin())
        Serial.println(http.getErrorMessage());

    cam.mDNS("esp32cam");
    Serial.println(http.getWelcomeMessage());


    /**
     * Camera setting
     */
    sensor_t * s = esp_camera_sensor_get();
    s->set_brightness(s, 0);     // -2 to 2
    s->set_contrast(s, 0);       // -2 to 2
    s->set_saturation(s, 0);     // -2 to 2
    s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
//    s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
//    s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
//    s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
//    s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
//    s->set_aec2(s, 0);           // 0 = disable , 1 = enable
//    s->set_ae_level(s, 0);       // -2 to 2
//    s->set_aec_value(s, 300);    // 0 to 1200
//    s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
//    s->set_agc_gain(s, 0);       // 0 to 30
//    s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
//    s->set_bpc(s, 0);            // 0 = disable , 1 = enable
//    s->set_wpc(s, 1);            // 0 = disable , 1 = enable
//    s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
//    s->set_lenc(s, 1);           // 0 = disable , 1 = enable
//    s->set_hmirror(s, 0);        // 0 = disable , 1 = enable
//    s->set_vflip(s, 0);          // 0 = disable , 1 = enable
//    s->set_dcw(s, 1);            // 0 = disable , 1 = enable
//    s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
}


void loop() {
    http.handle();

    if (detector.detect(decoder)) {
        Serial.print("Blob detected from top-left ");
        Serial.print(detector.blob.top);
        Serial.print(", ");
        Serial.print(detector.blob.left);
        Serial.print(" to bottom-right ");
        Serial.print(detector.blob.bottom);
        Serial.print(", ");
        Serial.println(detector.blob.right);
        Serial.print("Blob detection run in ");
        Serial.print(detector.getExecutionTimeInMillis());
        Serial.println("ms");
    }
}
