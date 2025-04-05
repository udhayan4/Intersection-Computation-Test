
#include <halp/controls.hpp>

struct ShakeGesture {
  
  halp::val_port<"Accel", vec3f> acceleration;

  struct {
    halp::knob<"Sensitivity", float> sensitivity{0.7};
    halp::knob<"Threshold", float> threshold{2.5}; 
  } controls;
 halp::trigger<"Detected"> detected;

  void operator()(const vec3f& accel) {
    const float magnitude = length(accel);
    
    if(magnitude > controls.threshold * controls.sensitivity) {
      detected.fire();
    }
  }
};

template<typename T>
struct GestureProcessor : T {
  
  halp::val_port<"Input", vec3f> sensor_input;
  halp::trigger<"Output"> gesture_output;

  void update() {
    this->operator()(sensor_input.value);
  }
};

struct Tilt : GestureProcessor<TiltGesture> {
    static consteval auto name() { return "Tilt Detector"; }
    static consteval auto category() { return "Gestures"; }
  };
  


struct ShakeDetector : public halp::object_base<ShakeDetector> {  
  puara::ShakeDetector detector;  
  halp::val_port<"Intensity", float> intensity;  
  halp::val_port<"Threshold", float> threshold{0.5};  

  void operator()(const halp::tick& t) {  
      
    intensity = detector.process(input_sensor_data, threshold);  
  }  
};  
TEST_CASE("ShakeDetector Basic Test", "[shake]") {  
    ShakeDetector plugin;  
    plugin.threshold = 0.5;  
    std::vector<float> violent_shake = {2.5, 3.0, 2.8};  
    plugin.process(violent_shake);  
    REQUIRE(plugin.intensity > 0.5);  
    std::vector<float> gentle_move = {0.1, 0.2, 0.3};  
    plugin.process(gentle_move);  
    REQUIRE(plugin.intensity < 0.5);  
  }  

if (typeof DeviceMotionEvent !== 'undefined' && DeviceMotionEvent.requestPermission) {  
    DeviceMotionEvent.requestPermission()  
      .then(permission => {  
        if (permission === 'granted') {  
          window.addEventListener('devicemotion', handleMotion);  
        }  
});  
const qrcode = require('qrcode');  
qrcode.toDataURL('ws://192.168.1.10:8080', (err, url) => {  
  document.getElementById('qrcode').src = url;  
});  

Feedback Generation sample code  : 

import oscP5.*;  
OscP5 oscP5;  

void setup() {  
  oscP5 = new OscP5(this, 8080);  
}  

void oscEvent(OscMessage msg) {  
  if (msg.addrPattern().equals("/tilt")) {  
    float x = msg.get(0).floatValue();  
    float y = msg.get(1).floatValue();  
    ellipse(x * width, y * height, 50, 50);  
  }  
}  

function showGesturePrompt(detectedGesture) {  
    const response = confirm(`Was that a ${detectedGesture}?`);  
    if (response) saveToDatabase();  
    else adjustThresholds();  
  } 

  if ('WebTransport' in window) {  
    const transport = new WebTransport('https://desktop-ip:port/webtransport');  
  } else {  
     
    const socket = new WebSocket('ws://desktop-ip:8080');  
  } 


  def test_load_rockstar_preset():  
    load_preset("presets/Rockstar/config.json")  
    assert node_exists("/synth/distortion")  
    assert connection_exists("shake/intensity", "/synth/distortion")  

    def encode_preset(preset):  
    emoji_str = GESTURE_EMOJI[preset["gesture"]]  
    emoji_str += TARGET_EMOJI[preset["target"]]  
    # Add parameters and values...  
    return emoji_str  ;}

    Rectangle {  
        width: 100; height: 100  
        color: "transparent"  
        property real angle: 0  
        property real sensitivity: 0.5  
       
        Canvas {  
          anchors.fill: parent  
          onPaint: {  
            var ctx = getContext("2d");  
            ctx.clearRect(0, 0, width, height);  
            ctx.beginPath();  
            ctx.arc(50, 50, 40, -Math.PI/2, (-Math.PI/2) + (angle * Math.PI/180));  
            ctx.strokeStyle = "blue";  
            ctx.lineWidth = 5;  
            ctx.stroke();  
          }  
        }  
        let shakeCount = 0;


        window.addEventListener("devicemotion", (e) => {
          const strength = Math.abs(e.acceleration.x + e.acceleration.y);
          
          
          if(strength > 15) {
            showPopup("Is this a SHAKE gesture?");
          }
        });
        
        function saveAsShake() {
          
          enableShakeTrigger();}

          class ShakeProcessor : public ossia::qt_process
          {
            Q_OBJECT
            Q_PROPERTY(bool active READ active NOTIFY activeChanged)
            
          public:
            ossia::outlet<float> intensityOut{this};
            
            void onShakeDetected(float intensity) {
              m_active = true;
              intensityOut->write_value(intensity);
              Q_EMIT activeChanged();
              
              QTimer::singleShot(200, [this] { m_active = false; });
            }
          
          signals:
            void activeChanged();
          
          private:
            bool m_active = false;
          };
          {  
            emoji: {  
            "gesture": "shake",  
            "target": "/synth/volume",  
            "mapping": "intensity + 1.5"  
          },  
            emoji: {  
            "gesture": "tilt",  
            "target": "/lights/brightness",  
            "mapping": "angle * 30"  
          }  
        }  
        
        Rectangle {  
            id: blob  
            width: 50 + (intensity * 100) 
            height: width  
            radius: width / 2  
            x: tiltX * parent.width  
            y: tiltY * parent.height  
            gradient: Gradient {  
              GradientStop { position: 0; color: "blue" }  
              GradientStop { position: 1; color: "red" }  
            }  
            property real intensity: 0  
            property real tiltX: 0  
            property real tiltY: 0  
          }  
Slider {  
            id: hapticSlider  
            from: 0; to: 1000  
            onMoved: osc.send("/haptic/max_duration", value)  
          }  
          
          
          function triggerHaptic(intensity) {  
            const pattern = intensity > 0.8 ? [200] : [100, 50, 100];  
            navigator.vibrate(pattern);  
          }  



