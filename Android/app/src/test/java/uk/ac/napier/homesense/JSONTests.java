package uk.ac.napier.homesense;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.junit.Test;
import static org.junit.Assert.*;

public class JSONTests {
    @Test
    public void JSONTest1(){
        JSONObject actual = new JSONObject();
        try {
            actual.put("id", 100);
            actual.put("name", "Jake");
            actual.put("age", 21);
            assertEquals(100, actual.getInt("id"));
            assertEquals("Jake", actual.getString("name"));
            assertEquals(21, actual.getInt("age"));
        } catch (Exception e){
            e.printStackTrace();
        }

    }
    @Test
    public void JSONTest2() {
        JSONObject json = new JSONObject();
        JSONArray jArray = new JSONArray();
        int colour[] = {100,200,0};
        for(int i=0; i < colour.length; i++){
            jArray.put(colour[i]);
        }
        try {
            json.put("colour", jArray);
            String test = json.getJSONArray("colour").toString();
            assertEquals("[100,200,0]" , test);
        } catch (JSONException e) {
            e.printStackTrace();
        }

        //TODO: MORE JSON TESTS
    }
}
