package uk.ac.napier.homesense;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;
import org.eclipse.paho.client.mqttv3.*;
import org.json.JSONObject;

import org.json.*;
import helpers.MQTTHelper;


public class MainActivity extends AppCompatActivity {
    MQTTHelper mqttHelper;
    TextView room;
    TextView temp;
    TextView humidity;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        temp = findViewById(R.id.temp);
        room = findViewById(R.id.room);
        humidity = findViewById(R.id.humidity);
        startMqtt();

    }
    private void startMqtt() {
        mqttHelper = new MQTTHelper(getApplicationContext());
        mqttHelper.setCallback(new MqttCallbackExtended() {
            @Override
            public void connectComplete(boolean b, String s) {

            }

            @Override
            public void connectionLost(Throwable throwable) {

            }

            @Override
            public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
                //Log.w("Debug", mqttMessage.toString());
                String msgString = mqttMessage.toString();
                String var;
                try {
                    JSONObject msgJSON = new JSONObject(msgString);
                    var = "Temp: " + msgJSON.getString("temp") + " C";
                    temp.setText(var);
                    var = "Humidity: " + msgJSON.getString("humidity") + "%";
                    humidity.setText(var);
                    var = msgJSON.getString("room");
                    room.setText(var);
                } catch (Exception e){
                    e.printStackTrace();
                }
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

            }
        });
    }

    public void redButtonClick(View view) {
        int red[] = {255,0,0};
        sendMessage(red);

    }

    public void greenButtonClick(View view) {
        int green[] = {0,255,0};
        sendMessage(green);
    }

    public void blueButtonClick(View view) {
        int blue[] = {0,0,255};
        sendMessage(blue);
    }

    public void sendMessage(int[] colour){
        MqttMessage message = new MqttMessage();
        JSONObject json = new JSONObject();
        JSONArray jArray = new JSONArray();
        for(int i=0; i < colour.length; i++){
            jArray.put(colour[i]);
        }
        try {
            json.put("colour", jArray);
            message.setPayload(json.toString().getBytes());
            try {
                mqttHelper.mqttAndroidClient.publish("esp/kitchen", message);
            } catch (MqttException e) {
                e.printStackTrace();
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        Log.w("Debug", json.toString());
    }

}
