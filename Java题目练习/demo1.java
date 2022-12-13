import javax.swing.*;
import java.awt.*;

public class Demo{
    public static void main(String[] args) {
        new Menu();
    }
}

class Menu extends JFrame{
    JPanel jp1, jp2;
    JLabel jl;
    JButton loginBtn, registerBtn, updateBtn, exitBtn;

    public Menu(){
        jp1 = new JPanel();
        jp2 = new JPanel();

        jl = new JLabel("菜单界面");

        loginBtn = new JButton("登    录");
        registerBtn = new JButton("注    册");
        updateBtn = new JButton("修改密码");
        exitBtn = new JButton("退    出");

        jl.setFont(new Font("Kaiti", Font.BOLD, 50));
        jp1.add(jl);
        loginBtn.setFont(new Font("Kaiti", Font.PLAIN, 20));
        registerBtn.setFont(new Font("Kaiti", Font.PLAIN, 20));
        updateBtn.setFont(new Font("Kaiti", Font.PLAIN, 20));
        exitBtn.setFont(new Font("Kaiti", Font.PLAIN, 20));
        jp2.setLayout(new GridLayout(4, 1, 0, 40));
        jp2.add(loginBtn);
        jp2.add(registerBtn);
        jp2.add(updateBtn);
        jp2.add(exitBtn);

        this.setLayout(new BorderLayout(0, 50));
        this.add(jp1, BorderLayout.NORTH);
        this.add(jp2, BorderLayout.CENTER);
        this.setBounds(300, 300, 800, 600);
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setVisible(true);
    }
}