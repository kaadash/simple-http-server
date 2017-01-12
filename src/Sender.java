import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Sender {

    public static void sendGET(String url, int port, String file) {
        try {
            Socket soc = new Socket(url, port);
            PrintWriter wtr = new PrintWriter(soc.getOutputStream());

            wtr.print("GET /" + file + " HTTP/1.1\r\n\r\n");
            wtr.flush();

            BufferedReader bufRead = new BufferedReader(new InputStreamReader(soc.getInputStream()));

            Window.setHeaders(read(bufRead));
            bufRead.readLine();
            Window.setInformation(read(bufRead));
            bufRead.close();
            wtr.close();

        } catch (Exception e) {

        }
    }
    public static void sendHEAD(String url, int port, String file){
        try {
            Socket soc = new Socket(url, port);

            PrintWriter wtr = new PrintWriter(soc.getOutputStream());

            wtr.print("HEAD /" + file + " HTTP/1.1\r\n\r\n");
            wtr.flush();

            BufferedReader bufRead = new BufferedReader(new InputStreamReader(soc.getInputStream()));

            Window.setHeaders(read(bufRead));
            bufRead.close();
            wtr.close();
        } catch (Exception e) {

        }
    }
    public static void sendPUT(String url, int port, String file) {
        try {
            Socket soc = new Socket(url, port);

            PrintWriter wtr = new PrintWriter(soc.getOutputStream());

            wtr.print("PUT /" + file + " HTTP/1.1\r\n");
            wtr.print("Content-Length: " + Window.getInformation().length() + "\r\n\r\n");
            wtr.print(Window.getInformation() + "\r\n");
            wtr.flush();

            wtr.close();
        } catch (Exception e) {

        }
    }
    public static void sendDELETE(String url, int port, String file) {
        try {
            Socket soc = new Socket(url, port);

            PrintWriter wtr = new PrintWriter(soc.getOutputStream());

            wtr.print("DELETE /" + file + " HTTP/1.1\r\n\r\n");
            wtr.flush();

            BufferedReader bufRead = new BufferedReader(new InputStreamReader(soc.getInputStream()));

            Window.setHeaders(read(bufRead));
            bufRead.close();
            wtr.close();
        } catch (Exception e) {

        }
    }
    private static String read(BufferedReader bufRead) {
        String message = "";
        try {
            String response;

            while(!(response = bufRead.readLine()).equals("")){
                message += response + '\n';
            }
        } catch (Exception e) {

        }
        return message;
    }
}
