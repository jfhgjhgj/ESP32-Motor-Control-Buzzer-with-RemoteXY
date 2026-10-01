import time
import requests

# عنوان الـ IP الخاص بالروبوت (تأكد منه من الشاشة التسلسلية Serial Monitor أو افتراضيًا إذا كان متصلاً بنقطة الاتصال)
# في العادة يكون عنوان الـ AP الافتراضي لـ ESP32 هو 192.168.4.1
ROBOT_IP = "192.168.4.1"
BASE_URL = f"http://{ROBOT_IP}/control"


def send_command(cmd):
  """إرسال أمر الحركة للروبوت"""
  try:
    response = requests.get(BASE_URL, params={"cmd": cmd}, timeout=2)
    if response.status_code == 200:
      print(f"تم إرسال الأمر بنجاح: {cmd}")
    else:
      print(f"فشل إرسال الأمر، رمز الاستجابة: {response.status_code}")
  except requests.exceptions.RequestException as e:
    print(f"حدث خطأ في الاتصال بالروبوت: {e}")


if __name__ == "__main__":
  # تأكد أولاً من اتصال حاسوبك بشبكة الـ Wi-Fi الخاصة بالروبوت: ESP32-Robot
  print("جاري بدء التحكم بالروبوت...")

  # 1. التحرك للأمام لمدة ثانيتين
  print("التحرك للأمام (Up)...")
  send_command("up")
  time.sleep(2)

  # 2. التوقف لمدة ثانية
  print("توقف (Stop)...")
  send_command("stop")
  time.sleep(1)

  # 3. التحرك للخلف لمدة ثانيتين
  print("التحرك للخلف (Down)...")
  send_command("down")
  time.sleep(2)

  # 4. الاستدارة يمينًا
  print("الاستدارة يمينًا (Right)...")
  send_command("right")
  time.sleep(1.5)

  # 5. التوقف النهائي
  print("إيقاف الروبوت نهائياً...")
  send_command("stop")
