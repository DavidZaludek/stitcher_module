/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package panoramadisplay;

import java.awt.BorderLayout;
import java.awt.Image;
import java.io.IOException;
import java.net.URL;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

/**
 *
 * @author davidzaludek
 */
public class PanoramaDisplay {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        JFrame frame = new JFrame();
        JPanel mainPanel = new JPanel(new BorderLayout());
        JLabel lblimage = new JLabel();
        
                mainPanel.add(lblimage);
                frame.add(mainPanel);
                frame.setVisible(true);
        
        while (true) {
            Image image = null;
            try {
                URL url = new URL("http://195.113.232.52:8080/stream.jpg");
                image = ImageIO.read(url);
                
                lblimage.setIcon(new ImageIcon(image));


                Thread.sleep(10);
            } catch (IOException | InterruptedException e) {
            }
        }
    }

}
