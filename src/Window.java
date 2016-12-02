import javafx.collections.FXCollections;
import javafx.geometry.Insets;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.scene.control.TextArea;


public class Window {
    private static GridPane gridPane = new GridPane();
//    private static final Button button = new Button();
    public static GridPane getGridPane() {
        return gridPane;
    }
    public static void prepare(){
        gridPane.setPadding(new Insets(20, 50, 20, 50));
        gridPane.setHgap(10);
        gridPane.setVgap(10);

        ChoiceBox request = new ChoiceBox(FXCollections.observableArrayList(
                "GET", "HEAD", "PUT", "DELETE"));
        request.getSelectionModel().selectFirst();

        TextField address = new TextField();
        address.setPromptText("IP");
//        address.setPrefColumnCount(10);
//        address.getText();

        TextField port = new TextField();
        port.setPromptText("Port");

        TextField file = new TextField();
        file.setPromptText("File");

        Button button = new Button();
        button.setText("Send!");
        button.setOnAction(e -> {
            try {
                Sender.sendRequest(address.getText(), Integer.valueOf(port.getText()),
                        file.getText(), request.getSelectionModel().getSelectedIndex());
            } catch (Exception ee) {

            }
        });

        TextArea headerSection = new TextArea();
        headerSection.setEditable(false);
        headerSection.setMinWidth(800);

        TextArea information = new TextArea();
        information.setEditable(false);
        information.setMinWidth(800);

        gridPane.add(request, 0, 0, 1, 1);
        gridPane.add(address, 1, 0, 1, 1);
        gridPane.add(port, 2, 0, 1, 1);
        gridPane.add(file, 3, 0, 1, 1);
        gridPane.add(button, 4, 0, 1, 1);
        gridPane.add(headerSection, 0, 1, 4, 1);
        gridPane.add(information, 0, 2, 4, 3);
    }

}
