/**
 * Created by ronisko on 24.11.16.
 */
import java.io.*;
import java.net.*;

public class Client {


    public static void main(String[] args) throws Exception {

        Client client = new Client();

        client.sendGET("localhost", 1234, "2.html");
        //client.sendDEL("localhost", 1234, "2.html");
    }

    private void sendGET(String url, int port, String file) throws Exception {
        System.out.println("Sending GET request.\nResponse:\n");
        Socket soc = new Socket(url, port);

        PrintWriter wtr = new PrintWriter(soc.getOutputStream());

        wtr.println("GET /" + file + " HTTP/1.1");
        wtr.flush();

        BufferedReader bufRead = new BufferedReader(new InputStreamReader(soc.getInputStream()));
        String response;

        while((response = bufRead.readLine()) != null){
            System.out.println(response);
        }

        bufRead.close();
        wtr.close();
    }

    private void sendDEL(String url, int port, String file) throws Exception {
        System.out.println("Sending DELETE request.");
        Socket soc = new Socket(url, port);

        PrintWriter wtr = new PrintWriter(soc.getOutputStream());

        wtr.println("DELETE /" + file + " HTTP/1.1");
        wtr.flush();
        wtr.close();
    }

}