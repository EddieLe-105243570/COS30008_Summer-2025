To actually run this, you need to:

🧱 Bước 1: Tạo Project Visual Studio
Mở Visual Studio

Chọn Create a new project

Chọn Console App (C++)

Đặt tên: ProblemSet1

Chọn nơi lưu phù hợp (thư mục chứa tất cả các file bạn có)

👉 Project sẽ tạo ra file ProblemSet1.cpp mặc định → bạn có thể xoá nó hoặc bỏ qua.

📂 Bước 2: Thêm file vào project
Vào Solution Explorer:
Chuột phải vào Project name → Add → Existing Item

Chọn và thêm các file sau (toàn bộ đều nằm trong thư mục bạn có):
/data, /src, /include

⚙️ Bước 3: Cấu hình đường dẫn Include
Chuột phải vào project → Properties

Vào mục:

Configuration Properties → C/C++ → General → Additional Include Directories
Add đường dẫn đến thư mục include/
→ VD: nếu thư mục include nằm cạnh project, thì thêm:

$(ProjectDir)include

🧪 Bước 4: Chạy thử chương trình
Cách đơn giản nhất:
Chuột phải vào project → chọn Set as Startup Project

Chuột phải vào Main.cpp → Set as StartUp File

Nhấn Ctrl + F5 để chạy

⚠️ Nhưng: main() trong Main.cpp đang yêu cầu file input qua argv[1]
✅ Cấu hình Command Arguments:
Vào Project → Properties

Đi tới:

Configuration Properties → Debugging
Ở ô Command Arguments, nhập:

data/Parallelogram.txt
(hoặc đường dẫn đúng tới file nếu không nằm cùng cấp)
