import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

import java.util.ArrayList;
import java.util.List;

public class MouseRecorderB extends Application {
    private List<double[]> coordinates = new ArrayList<>();
    private boolean recording = false;
    private int index = 0;
    private Canvas canvas;
    private GraphicsContext gc;

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        canvas = new Canvas(800, 600);
        gc = canvas.getGraphicsContext2D();

        Button startButton = new Button("Start Recording");
        startButton.setOnAction(event -> {
            recording = true;
            coordinates.clear();
            index = 0;
        });

        Button stopButton = new Button("Stop Recording");
        stopButton.setOnAction(event -> recording = false);

        Button replayButton = new Button("Replay");
        replayButton.setOnAction(event -> {
            index = 0;
            AnimationTimer timer = new AnimationTimer() {
                @Override
                public void handle(long now) {
                    if (index < coordinates.size()) {
                        gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
                        double[] point = coordinates.get(index);
                        gc.setFill(Color.BLUE);
                        gc.fillOval(point[0], point[1], 5, 5);
                        index++;
                    } else {
                        stop();
                    }
                }
            };
            timer.start();
        });

        canvas.setOnMouseMoved(event -> {
            if (recording) {
                coordinates.add(new double[]{event.getX(), event.getY()});
            }
        });

        VBox root = new VBox();
        root.getChildren().addAll(canvas, startButton, stopButton, replayButton);

        Scene scene = new Scene(root, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
