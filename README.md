# QR-Code-Filter-for-XPS-Printer-Driver

This project is to develop a QR code filter for XPS Printer Driver. This QR code filter can:
1.	Read/parse the contents of the printed file.
2.	Collect all the contents of the printed file.
3.	Encode the collected contents to QR code.
4.	Create and add a new page as the last page of the printed file.
5.	Display the generated QR code to the new created last page.

When the user prints the file by using this QR Code Printer Driver, a new extra page will be added to the last page of the file. A QR code will be display at this new extra page. The size and position of the QR code can be adjusted through QR code UI before printing. When the user scans the QR code, it will get back the content of the file (without keeping the font size and spacing).

This project is done by modifying the code as below:
1.	XPS Printer Driver - https://github.com/Microsoft/Windows-driver-samples/tree/master/print/XPSDrvSmpl
2.	QR code generator - https://github.com/nayuki/QR-Code-generator



**Note: The code given is only the part of the project, they are either newly written or modified from the code as shown in above link.
**Attention: This project is just used for academic purpose, install it with your own risk.
