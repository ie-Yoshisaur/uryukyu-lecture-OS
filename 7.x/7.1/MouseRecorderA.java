import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Scene;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.control.Button;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.stage.Stage;
import javafx.util.Duration;

import java.util.ArrayList;
import java.util.List;

public class MouseRecorderA extends Application {
    private List<double[]> coordinates = new ArrayList<>();
    private boolean recording = false;
    private int timelineIndex = 0;
    private int frameIndex = 0;
    private Canvas canvas;
    private GraphicsContext gc;
    private double prevX = 0;
    private double prevY = 0;
    private double currentX = 0;
    private double currentY = 0;

    public static void main(String[] args) {
        launch(args);
    }

    Timeline timeline = new Timeline(
            new KeyFrame(Duration.millis(1), new EventHandler<ActionEvent>() {
                @Override
                public void handle(ActionEvent event) {
                    if (timelineIndex < coordinates.size()) {
                        gc.clearRect(0, 0, canvas.getWidth(), canvas.getHeight());
                        double[] point = coordinates.get(timelineIndex);
                        gc.setFill(Color.BLUE);
                        gc.fillOval(point[0], point[1], 5, 5);
                        timelineIndex++;
                    } else {
                        timeline.stop();
                    }
                }
            })
    );

    @Override
    public void start(Stage primaryStage) {
        canvas = new Canvas(800, 600);
        gc = canvas.getGraphicsContext2D();

        Button startButton = new Button("Start Recording");
        startButton.setOnAction(event -> {
            recording = true;
            coordinates.clear();
            timelineIndex = 0;
        });

        Button stopButton = new Button("Stop Recording");
        stopButton.setOnAction(event -> recording = false);

        Button replayButton = new Button("Replay");
        replayButton.setOnAction(event -> {
            timelineIndex = 0;
            timeline.setCycleCount(coordinates.size());
            timeline.play();
        });

        canvas.setOnMouseMoved(event -> {
            if (recording) {
                frameIndex = 0;
                currentX = event.getX();
                currentY = event.getY();
                if (prevX == 0 && prevY == 0) {
                    for (frameIndex = 0; frameIndex < (16); frameIndex++) {
                        coordinates.add(new double[]{currentX, currentY});
                    }
                } else {
                    for (frameIndex = 0; frameIndex < (16); frameIndex++) {
                        coordinates.add(new double[]{((currentX - prevX) * ((frameIndex + 1) / (16))) + prevX, ((currentY - prevY) * ((frameIndex + 1) / (16))) + prevY});
                    }
                }
                prevX = currentX;
                prevY = currentY;
            }
        });

        VBox root = new VBox();
        root.getChildren().addAll(canvas, startButton, stopButton, replayButton);

        Scene scene = new Scene(root, 800, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }
}
