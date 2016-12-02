import javafx.application.Platform;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Sender {
    public static void sendRequest(String url, int port, String file, int choice) throws Exception{
        switch (choice) {
            case 0:
                sendGET(url, port, file);
                break;
            case 1:
                sendHEAD(url, port, file);
                break;
            case 2:
                break;
            case 3:
                sendDELETE(url, port, file);
                break;
        }
    }
    private static void sendGET(String url, int port, String file) throws Exception {
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
    private static void sendHEAD(String url, int port, String file) throws Exception {
        System.out.println("Sending HEAD request.\nResponse:\n");
        Socket soc = new Socket(url, port);

        PrintWriter wtr = new PrintWriter(soc.getOutputStream());

        wtr.println("HEAD /" + file + " HTTP/1.1");
        wtr.flush();

        BufferedReader bufRead = new BufferedReader(new InputStreamReader(soc.getInputStream()));
        String response;

        while((response = bufRead.readLine()) != null){
            System.out.println(response);
        }

        bufRead.close();
        wtr.close();
    }
    private static void sendDELETE(String url, int port, String file) throws Exception {
        System.out.println("Sending DELETE request.");
        Socket soc = new Socket(url, port);

        PrintWriter wtr = new PrintWriter(soc.getOutputStream());

        wtr.println("DELETE /" + file + " HTTP/1.1");
        wtr.flush();
        wtr.close();
    }
}
