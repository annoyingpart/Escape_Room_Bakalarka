module com.example.a0 {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.net.http;


    opens com.example.a0 to javafx.fxml;
    exports com.example.a0;
}