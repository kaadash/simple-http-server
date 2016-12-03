import javafx.application.Platform;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Sender {

    public static void sendGET(String url, int port, String file) {
            try {
                System.out.println("Sending GET request.\nResponse:\n");
                Socket soc = new Socket(url, port);
                PrintWriter wtr = new PrintWriter(soc.getOutputStream());

                wtr.println("GET /" + file + " HTTP/1.1");
                wtr.flush();

                BufferedReader bufRead = new BufferedReader(new InputStreamReader(soc.getInputStream()));
                String response;
                String message = new String();
                while((response = bufRead.readLine()) != null){
                    message += response + '\n';
                }
                Window.setInformation(message);
                bufRead.close();
                wtr.close();
            } catch (Exception e) {

            }
    }
    public static void sendHEAD(String url, int port, String file){
        try {
            System.out.println("Sending HEAD request.\nResponse:\n");
            Socket soc = new Socket(url, port);

            PrintWriter wtr = new PrintWriter(soc.getOutputStream());

            wtr.println("HEAD /" + file + " HTTP/1.1");
            wtr.flush();

            BufferedReader bufRead = new BufferedReader(new InputStreamReader(soc.getInputStream()));
            String response;
            String headers = new String();
            while((response = bufRead.readLine()) != null){
                headers += response + '\n';
            }
            Window.setHeaders(headers);
            bufRead.close();
            wtr.close();
        } catch (Exception e) {

        }
    }
    public static void sendPUT(String url, int port, String file) {
        try {
            System.out.println("Sending PUT request.\nResponse:\n");
            Socket soc = new Socket(url, port);

            PrintWriter wtr = new PrintWriter(soc.getOutputStream());

            wtr.println("PUT /" + file + " HTTP/1.1");
            wtr.flush();

            wtr.close();
        } catch (Exception e) {

        }
    }
    public static void sendDELETE(String url, int port, String file) {
        try {
            System.out.println("Sending DELETE request.");
            Socket soc = new Socket(url, port);

            PrintWriter wtr = new PrintWriter(soc.getOutputStream());

            wtr.println("DELETE /" + file + " HTTP/1.1");
            wtr.flush();
            wtr.close();
        } catch (Exception e) {

        }
    }
}
