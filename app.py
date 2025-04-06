import streamlit as st
import pandas as pd
import plotly.express as px
import dashscope
from dashscope import Generation
import os
import requests



# Page config
st.set_page_config(page_title="Indoor Environmental Monitoring System", layout="wide")

# Load data
@st.cache_data
def load_data():
    df = pd.read_csv('stm32_data.csv')
    return df

df = load_data()

@st.cache_data(ttl=3600)  
def get_hongkong_weather():
    try:
      
        url = "https://data.weather.gov.hk/weatherAPI/opendata/weather.php?dataType=rhrread&lang=en"
        response = requests.get(url)
        data = response.json()
        
     
        temperature = float(data['temperature']['data'][0]['value'])
        humidity = float(data['humidity']['data'][0]['value'])
        
        return {
            'temperature': temperature,
            'humidity': humidity
        }
    except Exception as e:
        print(f"Failed to obtain weather data. Procedure: {str(e)}")
        return None

# Title
st.title("Indoor Environmental Monitoring System")
st.subheader("Real-time Temperature, Humidity & Brightness Monitor")

hk_weather = get_hongkong_weather()


if hk_weather:
    latest_data = df.iloc[-1]
    temp_diff = latest_data['temperature'] - hk_weather['temperature']
    hum_diff = latest_data['humidity'] - hk_weather['humidity']
    
    st.markdown("""
    <div style='background-color: #f0f2f6; padding: 10px; border-radius: 5px;'>
        <h3>🌍 Indoor vs Outdoor Comparison</h3>
        <p>Hong Kong Outdoor: Temperature {:.1f}°C, Humidity {:.1f}%</p>
        <p>Temperature Difference: {:.1f}°C | Humidity Difference: {:.1f}%</p>
    </div>
    """.format(
        hk_weather['temperature'],
        hk_weather['humidity'],
        temp_diff,
        hum_diff
    ), unsafe_allow_html=True)

# Create three columns layout for charts
col1, col2, col3 = st.columns(3)

with col1:
    # Temperature line chart
    fig_temp = px.line(df, x='index', y='temperature', 
                       title='Temperature Trend',
                       labels={'temperature': 'Temperature (°C)', 'index': 'Time'})
    # Set y-axis range for temperature
    fig_temp.update_layout(yaxis_range=[25, 30])
    st.plotly_chart(fig_temp)

with col2:
    # Humidity line chart
    fig_hum = px.line(df, x='index', y='humidity', 
                      title='Humidity Trend',
                      labels={'humidity': 'Humidity (%)', 'index': 'Time'})
    st.plotly_chart(fig_hum)

with col3:
    # Brightness line chart
    fig_bright = px.line(df, x='index', y='adcx', 
                        title='Brightness Trend',
                        labels={'adcx': 'Brightness', 'index': 'Time'})
    st.plotly_chart(fig_bright)

# Display current status
st.subheader("Current Status")
latest_data = df.iloc[-1]
col4, col5, col6 = st.columns(3)
col4.metric("🌡️ Temperature", f"{latest_data['temperature']}°C")
col5.metric("💧 Humidity", f"{latest_data['humidity']}%")
col6.metric("💡 Brightness", f"{latest_data['adcx']}")

st.subheader("🤖 Analysis & Recommendations")

temp_mean = df['temperature'].mean()
temp_std = df['temperature'].std()
temp_current = latest_data['temperature']

hum_mean = df['humidity'].mean()
hum_current = latest_data['humidity']


light_mean = df['adcx'].mean()
light_current = latest_data['adcx']

analysis_col1, analysis_col2, analysis_col3 = st.columns(3)

with analysis_col1:
    st.write("🌡️ **Temperature Analysis**")
    if temp_current > 28:
        st.warning("Temperature is slightly high. Consider:")
        st.write("- Adjusting air conditioning")
        st.write("- Opening windows for ventilation")
    elif temp_current < 26:
        st.info("Temperature is slightly low. Consider:")
        st.write("- Increasing room temperature")
    else:
        st.success("Temperature is in optimal range")

with analysis_col2:
    st.write("💧 **Humidity Analysis**")
    if hum_current > 70:
        st.warning("Humidity is high. Suggestions:")
        st.write("- Use dehumidifier")
        st.write("- Improve ventilation")
    elif hum_current < 40:
        st.warning("Humidity is low. Consider:")
        st.write("- Using a humidifier")
        st.write("- Adding indoor plants")
    else:
        st.success("Humidity level is comfortable")

with analysis_col3:
    st.write("💡 **Brightness Analysis**")
    if light_current < 50:
        st.warning("Light level is low. Suggestions:")
        st.write("- Increase artificial lighting")
        st.write("- Open curtains/blinds")
    elif light_current > 90:
        st.info("Light level is high. Consider:")
        st.write("- Reducing artificial lighting")
        st.write("- Adjusting window shades")
    else:
        st.success("Light level is optimal")


comfort_score = 100 - (
    abs(temp_current - 27) * 5 +  
    abs(hum_current - 55) * 0.5 +  
    abs(light_current - 70) * 0.2  
)
st.metric("🏠 Overall Environment Score", f"{max(0, min(100, round(comfort_score)))}%")

#
st.subheader("📊 Trend Analysis")
col7, col8 = st.columns(2)
with col7:
    st.write("**24-Hour Trends:**")
    temp_trend = df['temperature'].iloc[-1] - df['temperature'].iloc[-24] if len(df) > 24 else 0
    hum_trend = df['humidity'].iloc[-1] - df['humidity'].iloc[-24] if len(df) > 24 else 0
    st.write(f"- Temperature change: {temp_trend:.1f}°C")
    st.write(f"- Humidity change: {hum_trend:.1f}%")

with col8:
    st.write("**Recommendations for Optimal Comfort:**")
    if comfort_score < 80:
        st.write("- Adjust temperature to 26-28°C")
        st.write("- Maintain humidity between 40-60%")
        st.write("- Ensure adequate lighting (60-80 brightness level)")


if st.checkbox('Show Raw Data'):
    st.write(df)


os.environ['DASHSCOPE_API_KEY'] = 'YOUR-API'
dashscope.api_key = os.environ['DASHSCOPE_API_KEY']
dashscope.base_url = 'https://dashscope.aliyuncs.com/api/v1'


def get_ai_response(prompt, env_data):
    try:
        response = Generation.call(
            model='qwen-7b-chat',
            prompt=prompt,
            messages=[{
                'role': 'system',
                'content': f'You are an indoor environment monitoring assistant. Current environment data: Temperature {env_data["temperature"]}°C, Humidity {env_data["humidity"]}%, Brightness {env_data["adcx"]}'
            }, {
                'role': 'user',
                'content': prompt
            }]
        )
        return str(response.output.text) if response and response.output else "No response generated"
    except Exception as e:
        print(f"Error: {str(e)}")
        return f"Error occurred: {str(e)}"


st.subheader("💬 AI Assistant")
st.write("Ask questions about the environmental data or get personalized suggestions.")


if 'chat_history' not in st.session_state:
    st.session_state.chat_history = []


user_input = st.text_input("Ask a question:", key="user_input")

if st.button("Send"):
    if user_input:
      
        current_env = {
            'temperature': latest_data['temperature'],
            'humidity': latest_data['humidity'],
            'adcx': latest_data['adcx']
        }
        
  
        ai_response = get_ai_response(user_input, current_env)

        st.session_state.chat_history.append(("You", user_input))
        st.session_state.chat_history.append(("AI", ai_response))

chat_container = st.container()
with chat_container:
    for role, message in st.session_state.chat_history:
        if role == "You":
            st.write(f"👤 **You:** {message}")
        else:
            st.write(f"🤖 **AI:** {message}") 