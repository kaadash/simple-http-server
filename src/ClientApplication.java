import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

import java.io.*;
import java.net.*;

public class ClientApplication extends Application {


    public static void main(String[] args) throws Exception {
        launch(args);
//        ClientApplication client = new ClientApplication();

//        client.sendGET("localhost", 1234, "2.html");
        //client.sendDEL("localhost", 1234, "2.html");
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        primaryStage.setTitle("Client");
        Window.prepare();

        Scene scene = new Scene(Window.getGridPane(), 900, 600);
        primaryStage.setScene(scene);
        primaryStage.show();

    }
}