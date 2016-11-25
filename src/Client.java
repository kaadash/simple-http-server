/**
 * Created by ronisko on 24.11.16.
 */
import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class Client {

    public static void main(String[] args) throws Exception {

        Client http = new Client();

        System.out.println("Send Http DEL request");
        http.sendDel();

    }

    private void sendDel() throws Exception {

        String url = "http://localhost:1234/2.html";

        URL obj = new URL(url);
        HttpURLConnection con = (HttpURLConnection) obj.openConnection();

        con.setRequestMethod("DELETE");

        int responseCode = con.getResponseCode();
        System.out.println("\nSending 'DEL' request to URL : " + url);
        System.out.println("Response Code : " + responseCode);

    }
}