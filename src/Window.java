import javafx.application.Platform;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.collections.FXCollections;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.scene.layout.ColumnConstraints;
import javafx.scene.layout.GridPane;
import javafx.scene.control.TextArea;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;


public class Window {
    private static GridPane gridPane = new GridPane();
    private static TextArea headers = new TextArea();
    private static TextArea information = new TextArea();

    public static void setHeaders(String headers) {
        Window.headers.setText(headers);
    }

    public static void setInformation(String information) {
        Window.information.setText(information);
    }

    public static GridPane getGridPane() {
        return gridPane;
    }
    public static void prepare(){
        gridPane.setStyle("-fx-background-color: #303030");
        gridPane.setPadding(new Insets(20, 50, 20, 50));
        gridPane.setHgap(25);//*4
        gridPane.setVgap(20);
        ColumnConstraints cc1 = new ColumnConstraints(100);
        ColumnConstraints cc2 = new ColumnConstraints(230);
        ColumnConstraints cc3 = new ColumnConstraints(80);
        ColumnConstraints cc4 = new ColumnConstraints(230);
        ColumnConstraints cc5 = new ColumnConstraints(100);
        gridPane.getColumnConstraints().addAll(cc1, cc2, cc3, cc4, cc5);
        ChoiceBox request = new ChoiceBox(FXCollections.observableArrayList(
                "GET", "HEAD", "PUT", "DELETE"));
        request.getSelectionModel().selectFirst();

        TextField address = new TextField();
        address.setPromptText("IP");

        TextField port = new TextField();
        port.setPromptText("Port");

        TextField file = new TextField();
        file.setPromptText("File");

        Button button = new Button();
        button.setText("Send!");
        button.setOnAction(e -> {
            new Thread(() -> {
                    setHeaders("");
                    setInformation("");
                    switch (request.getSelectionModel().getSelectedIndex()) {
                        case 0:
                            Sender.sendGET(address.getText(), Integer.valueOf(port.getText()), file.getText());
                            break;
                        case 1:
                            Sender.sendHEAD(address.getText(), Integer.valueOf(port.getText()), file.getText());
                            break;
                        case 2:
                            Sender.sendPUT(address.getText(), Integer.valueOf(port.getText()), file.getText());
                            break;
                        case 3:
                            Sender.sendDELETE(address.getText(), Integer.valueOf(port.getText()), file.getText());
                            break;
                    }
            }).start();
        });

        Text text1 = new Text("headers:");
        Text text2 = new Text("body:");
        Text text3 = new Text("~Sieci Komputerowe~");
        text1.setStyle("-fx-font-size: 30px");
        text1.setFill(Color.RED);
        text2.setStyle("-fx-font-size: 30px");
        text2.setFill(Color.RED);
        text3.setStyle("-fx-font-size: 50px");
        text3.setFill(Color.RED);

        headers.setEditable(false);
        headers.setMinWidth(800);

        information.setEditable(false);
        information.setMinWidth(800);

        gridPane.add(request, 0, 0, 1, 1);
        gridPane.add(address, 1, 0, 1, 1);
        gridPane.add(port, 2, 0, 1, 1);
        gridPane.add(file, 3, 0, 1, 1);
        gridPane.add(button, 4, 0, 1, 1);
        gridPane.add(text1, 0, 1, 1, 1);
        gridPane.add(headers, 0, 2, 4, 1);
        gridPane.add(text2, 0, 3, 1, 1);
        gridPane.add(information, 0, 4, 4, 3);
        gridPane.add(text3, 1, 8, 3, 3);
    }

}
