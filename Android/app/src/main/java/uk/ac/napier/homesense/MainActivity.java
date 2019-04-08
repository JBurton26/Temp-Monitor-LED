package uk.ac.napier.homesense;
import android.app.ActionBar;
import android.support.constraint.ConstraintLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.*;
import org.json.JSONObject;

import org.json.*;

import java.util.List;

import helpers.MQTTHelper;


public class MainActivity extends AppCompatActivity {
    MQTTHelper mqttHelper;
    TextView room;
    TextView temp;
    TextView humidity;
    TextView room2;
    TextView temp2;
    TextView humidity2;
    EditText redValue;
    EditText redValue2;
    EditText greenValue;
    EditText greenValue2;
    EditText blueValue;
    EditText blueValue2;
    //List<String> list;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        temp = findViewById(R.id.temp);
        room = findViewById(R.id.room);
        humidity = findViewById(R.id.humidity);
        temp2 = findViewById(R.id.temp2);
        room2 = findViewById(R.id.room2);
        humidity2 = findViewById(R.id.humidity2);
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
                Log.w("Debug", mqttMessage.toString());
                String msgString = mqttMessage.toString();


                try {
                    JSONObject msgJSON = new JSONObject(msgString);
                    String ifstate = msgJSON.getString("room");
                    Log.w("Debug",msgJSON.getString("room") );
                    if(ifstate.equals("Kitchen")){
                        String var;
                        Log.w("Debug", "Kitchen If triggered");
                        var = "Temp: " + msgJSON.getString("temp") + " C";
                        temp.setText(var);
                        var = "Humidity: " + msgJSON.getString("humidity") + "%";
                        humidity.setText(var);
                        var = msgJSON.getString("room");
                        room.setText(var);
                    } else if (ifstate.equals("Bedroom")){
                        String var2;
                        Log.w("Debug", "Bedroom If triggered");
                        var2 = "Temp: " + msgJSON.getString("temp") + " C";
                        temp2.setText(var2);
                        var2 = "Humidity: " + msgJSON.getString("humidity") + "%";
                        humidity2.setText(var2);
                        var2 = msgJSON.getString("room");
                        room2.setText(var2);
                    }

                } catch (Exception e){
                    e.printStackTrace();
                }

            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

            }
        });
    }
    public void updateRoom1LED(View view){
        try{
            redValue = findViewById((R.id.redValue));
            greenValue = findViewById((R.id.greenValue));
            blueValue = findViewById((R.id.blueValue));
            int colour[] = {0,0,0};
            String topic = "esp/kitchen";
            colour[0] = Integer.parseInt(redValue.getText().toString());
            colour[1] = Integer.parseInt(greenValue.getText().toString());
            colour[2] = Integer.parseInt(blueValue.getText().toString());

            sendMessage(colour, topic);
        } catch (Exception e){
            e.printStackTrace();
            Toast toast = Toast.makeText(this,"Error Updating LED", Toast.LENGTH_SHORT);
            toast.show();
        }
    }
    public void updateRoom2LED(View view){
        try{
            redValue2 = findViewById((R.id.redValue2));
            greenValue2 = findViewById((R.id.greenValue2));
            blueValue2 = findViewById((R.id.blueValue2));
            int colour[] = {0,0,0};
            String topic = "esp/bedroom";
            colour[0] = Integer.parseInt(redValue2.getText().toString());
            colour[1] = Integer.parseInt(greenValue2.getText().toString());
            colour[2] = Integer.parseInt(blueValue2.getText().toString());

            sendMessage(colour, topic);
        } catch (Exception e){
            e.printStackTrace();
            Toast toast = Toast.makeText(this,"Error Updating LED", Toast.LENGTH_SHORT);
            toast.show();
        }
    }
    public void sendMessage(int[] colour, String topic){
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
                mqttHelper.mqttAndroidClient.publish(topic, message);
            } catch (MqttException e) {
                e.printStackTrace();
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
        Log.w("Debug", json.toString());
        Toast toast = Toast.makeText(this,"Message Sent",Toast.LENGTH_SHORT);
        toast.show();
    }

}
