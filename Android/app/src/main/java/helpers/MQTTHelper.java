package helpers;
//Import statements
import android.content.Context;
import android.util.Log;
import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.*;

public class MQTTHelper {
    public MqttAndroidClient mqttAndroidClient;
    final String serverUri = "tcp://broker.shiftr.io:1883"; //"protocol://brokerAddress:port"
    final String clientId = "JakePhone";    //What do I show up as on the shiftr.io visualiser?
    final String subscriptionTopic = "phone";   //Subscribes to one topic TODO: Add more subscription topics
    final String username = "cfc9ef9d"; //Username and Password for connecting to the MQTT Broker
    final String password = "aa8346a53372ae9f";

    public MQTTHelper(Context context){ //The context is "this"
        mqttAndroidClient = new MqttAndroidClient(context, serverUri, clientId); //(this, "protocol://brokerAddress:port", What I want to show up as)
        mqttAndroidClient.setCallback(new MqttCallbackExtended() {      //Method allowing the system to receive new messages without breaking it (According to Documentation)
            @Override
            public void connectComplete(boolean b, String s) { //Tells the Logcat :3 that the phone is connected to the MQTT Broker
                Log.w("MQTT", s);
            }
            @Override
            public void connectionLost(Throwable throwable) {
            }
            @Override
            public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception { //TODO: add unit test
                Log.w("MQTT", mqttMessage.toString()); //Tells the Logcat :3 that a message has arrived
            }
            @Override
            public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {   //Says when the message has finished sending I think TODO: Figure this out
                                                                                    //Was in documentation but not explained, will remove if deemed unnecessary in future
            }
        });
        connect(); //Calls the connect method which connects to the MQTT Broker
    }

    public void setCallback(MqttCallbackExtended callback) {    //Allows the program to collect more messages while running without breaking the program
        mqttAndroidClient.setCallback(callback);
    }

    private void connect(){ //Method connects to the MQTT Broker
        MqttConnectOptions mqttConnectOptions = new MqttConnectOptions(); //adds all of the settings for connecting to the MQTT Broker
        mqttConnectOptions.setAutomaticReconnect(true); //Solution to the Processing 3+ problem
        mqttConnectOptions.setCleanSession(false);
        mqttConnectOptions.setUserName(username); //Unique
        mqttConnectOptions.setPassword(password.toCharArray()); //Unique
        try { //throws fists when there is an issue connecting to the broker
            mqttAndroidClient.connect(mqttConnectOptions, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    DisconnectedBufferOptions disconnectedBufferOptions = new DisconnectedBufferOptions();
                    disconnectedBufferOptions.setBufferEnabled(true);
                    disconnectedBufferOptions.setBufferSize(100);
                    disconnectedBufferOptions.setPersistBuffer(false);
                    disconnectedBufferOptions.setDeleteOldestMessages(false);
                    mqttAndroidClient.setBufferOpts(disconnectedBufferOptions);
                    subscribeToTopic();
                }
                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.w("Mqtt", "Failed to connect to: " + serverUri + exception.toString());
                }
            });
        } catch (MqttException ex){
            ex.printStackTrace();
        }
    }

    private void subscribeToTopic() {   //Subscribes to one topic TODO: Subscribe to more than one topic
        try {
            mqttAndroidClient.subscribe(subscriptionTopic, 0, null, new IMqttActionListener() {
                @Override
                public void onSuccess(IMqttToken asyncActionToken) {
                    Log.w("Mqtt","Subscribed!");
                }

                @Override
                public void onFailure(IMqttToken asyncActionToken, Throwable exception) {
                    Log.w("Mqtt", "Subscribed fail!");
                }
            });
        } catch (MqttException ex) {
            System.err.println("Exceptionst subscribing");
            ex.printStackTrace();
        }
    }
}
