module com.example.a0 {
    requires javafx.controls;
    requires javafx.fxml;


    opens com.example.a0 to javafx.fxml;
    exports com.example.a0;
}