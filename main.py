import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QWidget, QLineEdit, QPushButton, QTextEdit
from PyQt5.QtCore import Qt
import socket

class CalculatorClient(QMainWindow):
    def __init__(self):
        super().__init__()

        self.initUI()
        self.server_address = ('127.0.0.1', 65432)

    def initUI(self):
        self.setWindowTitle('Arithmetic Client')
        self.setGeometry(100, 100, 400, 200)

        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        layout = QVBoxLayout()

        self.expression_input = QLineEdit(self)
        self.expression_input.setPlaceholderText("Enter expression (e.g., 3 + 4)")
        layout.addWidget(self.expression_input)

        self.result_display = QTextEdit(self)
        self.result_display.setReadOnly(True)
        layout.addWidget(self.result_display)

        self.send_button = QPushButton('Send', self)
        self.send_button.clicked.connect(self.send_expression)
        layout.addWidget(self.send_button)

        central_widget.setLayout(layout)

    def send_expression(self):
        expression = self.expression_input.text().strip()
        if expression:
            result = self.send_to_server(expression)
            self.result_display.append(f"Expression: {expression}\nResult: {result}\n")

    def send_to_server(self, expression):
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
                sock.connect(self.server_address)
                sock.sendall(expression.encode('utf-8'))
                data = sock.recv(1024).decode('utf-8')
                return data
        except ConnectionError:
            return "Server connection failed"

if __name__ == '__main__':
    app = QApplication(sys.argv)
    client = CalculatorClient()
    client.show()
    sys.exit(app.exec_())
