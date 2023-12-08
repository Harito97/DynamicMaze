import plotly.express as px
import plotly.subplots as subplot
import plotly.graph_objects as go
import numpy as np

def rounding(array):
    return np.round(array, 3)

def transformData(fileName):
    dataAnalysis = np.array([list(map(float, line.split())) for line in open(fileName, "r", encoding = "utf-8")])
    dataMedian = np.median(dataAnalysis, axis=0)
    return rounding(dataMedian), rounding(dataAnalysis)


def compareMedianData(data, name, title, title_x, title_y, nameFile):
    algorithms = ["AStarSerial", "AStarParallel", "ACOSerial", "ACOParallel"]
    fig = go.Figure()

    fig.add_trace(go.Bar(
        x=algorithms,
        y=data,
        name=name,
        marker=dict(color='#330C73'), opacity=0.75, text = data, textposition='auto'
    ))

    fig.add_trace(go.Scatter(
        x=algorithms,
        y=data,
        mode='lines+markers',
        name=name,
        line=dict(color='#EB89B5', width=2),
        marker=dict(color='#EB89B5', size=8)
    ))

    fig.update_layout(
        title={
            'text': title,
            'x': 0.5,
            'y': 0.9,
            'font': {'color': 'red'},
            'xanchor': 'center',
            'yanchor': 'top'
        },
        xaxis_title=title_x,
        yaxis_title=title_y,
        xaxis=dict(tickfont=dict(color='black', size=12)),
        yaxis=dict(tickfont=dict(color='black', size=12))
    )
    fig.write_image(r"backend\test\image_visualize\compareMedian{}.png".format(nameFile))


def outlierData(data, title_y, maze, nameFile):
    algorithms = ["AStarSerial", "AStarParallel", "ACOSerial", "ACOParallel"]
    colors = ["#FF6666", "#FFFF33", "#33FF33", "#33FFFF"]
    # Tạo hình vẽ với 1 hàng và 4 cột
    fig = subplot.make_subplots(rows=1, cols=4, horizontal_spacing=0.085)
    for i in range(data.shape[1]):
        fig.add_trace(go.Box(
            y=data[:, i],
            name=algorithms[i],
            marker=dict(color=colors[i])
        ), row=1, col=i+1)

    # Cấu hình layout
    fig.update_layout(
        title={
            'text': 'Outlier of each algorithm through 10 measurements on a ' + maze,
            'x': 0.5,
            'y': 0.9,
            'font': {'color': 'red'},
            'xanchor': 'center',
            'yanchor': 'top'
        },
        showlegend=False
    )

    # Cấu hình trục y cho từng biểu đồ
    fig.update_yaxes(title_text=title_y, row=1, col=1)

    # Cấu hình trục x cho từng biểu đồ
    fig.update_xaxes(title_text="", row=1, col=1)
    fig.update_xaxes(title_text="", row=1, col=2)
    fig.update_xaxes(title_text="", row=1, col=3)
    fig.update_xaxes(title_text="", row=1, col=4)

    fig.write_image(r"backend\test\image_visualize\outlier{}.png".format(nameFile))


def infomationThreads(data, name, color, index, nameFile):
    fig = go.Figure()
    x = ["Thread" + str(pow(2,i)) for i in range(1,9)]
    fig.add_trace(go.Bar(x=x,
                y=data[:,index],
                name=name,
                marker_color=color
                ))
    
    fig.update_layout(
    title={
            'text': name + ' running time on different threads',
            'x': 0.5,
            'y': 0.9,
            'font': {'color': 'red'},
            'xanchor': 'center',
            'yanchor': 'top'
        },
    xaxis_tickfont_size=14,
    xaxis=dict(title="Number Of Threads"),
    yaxis=dict(
        title="Run Time (ms)",
        titlefont_size=16,
        tickfont_size=14,
    ),
    legend=dict(
        x=0,
        y=1.0,
        bgcolor='rgba(255, 255, 255, 0)',
        bordercolor='rgba(255, 255, 255, 0)'
    ),
    barmode='group',
    bargap=0.15, 
    bargroupgap=0.1
    )

    fig.write_image(r"backend\test\image_visualize\{}Threads.png".format(nameFile))


def compareDiffMazes(data, nameFile):
    maze = ["10x10", "15x15", "20x20", "25x25", "50x50", "75x75", "100x100"]
    fig = go.Figure()

    fig.add_trace(go.Scatter(
        x=maze,
        y=data[:,1],
        line=dict(color='rgba(231,107,243,1)', width=2),
        mode="lines",
        name="AStar Parallel", text = data[:,0], textposition="top center"
    ))

    fig.add_trace(go.Scatter(
        x=maze,
        y=data[:,3],
        line=dict(color='rgba(0,176,246,1)', width=2),
        mode="lines",
        name="ACO Parallel", text = data[:,1], textposition="top center"
    ))

    fig.update_layout(
        title={
            'text': "Compare the AStar Parallel and ACO Parallel Algorithm on different dynamic mazes",
            'x': 0.5,
            'y': 0.9,
            'font': {'color': 'red'},
            'xanchor': 'center',
            'yanchor': 'top'
        },
        xaxis_title="Maze Size",
        yaxis_title="Run Time (ms)",
        xaxis=dict(tickfont=dict(color='black', size=12)),
        yaxis=dict(tickfont=dict(color='black', size=12))
    )
    fig.write_image(r"backend\test\image_visualize\compareDiffMaze{}.png".format(nameFile))


def findIntersection(data, nameFile):
    maze = ["10x10", "15x15", "20x20", "25x25", "50x50", "75x75", "100x100"]
    fig = go.Figure()

    fig.add_trace(go.Scatter(
        x=maze,
        y=data[:,0],
        line=dict(color='rgba(231,107,243,1)', width=2),
        mode="lines",
        name="AStar Serial", text = data[:,0]
    ))

    fig.add_trace(go.Scatter(
        x=maze,
        y=data[:,1],
        line=dict(color='rgba(0,176,246,1)', width=2),
        mode="lines",
        name="AStar Parallel", text = data[:,1]
    ))

    fig.update_layout(
        title={
            'text': "Compare the AStar Serial and AStar Parallel Algorithm on different dynamic mazes",
            'x': 0.5,
            'y': 0.9,
            'font': {'color': 'red'},
            'xanchor': 'center',
            'yanchor': 'top'
        },
        xaxis_title="Maze Size",
        yaxis_title="Run Time (ms)",
        xaxis=dict(tickfont=dict(color='black', size=12)),
        yaxis=dict(tickfont=dict(color='black', size=12))
    )
    fig.write_image(r"backend\test\image_visualize\intersection{}.png".format(nameFile))


def main():
    dataTime = transformData(r"backend\test\report_data\Report_Time.txt")
    dataMemory = transformData(r"backend\test\report_data\Report_Memory.txt")
    dataThreads = transformData(r"backend\test\report_data\Report_Threads.txt")
    dataTimeMaze = transformData(r"backend\test\report_data\Report_Time_Mazes.txt")
    dataTimeMazeDy = transformData(r"backend\test\report_data\Report_Time_MazeDy.txt")
    dataTimeDy = transformData(r"backend\test\report_data\Report_Time_Dynamic.txt")
    dataMemoryDy = transformData(r"backend\test\report_data\Report_Memory_Dynamic.txt")
    
    # So sánh giá trị median: time và memory của từng thuật toán trên maze tĩnh
    compareMedianData(dataTime[0], "Run Time", "Compare the running time of each algorithm on a static maze", "Algorithms", "Run Time (ms)", "TimeStatic")
    compareMedianData(dataMemory[0], "Memory", "Compare CPU memory usage of each algorithm on a statisc maze", "Algorithms", "Memory (MB)", "MemoryStatic")

    # So sánh giá trị median: time và memory của từng thuật toán trên maze động
    compareMedianData(dataTimeDy[0], "Run Time", "Compare the running time of each algorithm on a dynamic maze", "Algorithms", "Run Time (ms)", "TimeDynamic")
    compareMedianData(dataMemoryDy[0], "Memory", "Compare CPU memory usage of each algorithm on a dynamic maze", "Algorithms", "Memory (MB)", "MemoryDynamic")

    # Outlier của từng thuật toán với các giá trị time và memory trên maze tĩnh
    outlierData(dataTime[1], "Run Time (ms)", "static maze", "TimeStatic")
    outlierData(dataMemory[1], "Memory (MB)", "static maze", "MemoryStatic")

    
    # Outlier của từng thuật toán với các giá trị time và memory trên maze động
    outlierData(dataTimeDy[1], "Run Time (ms)", "dynamic maze", "TimeDynamic")
    outlierData(dataMemoryDy[1], "Memory (MB)", "dynamic maze", "MemoryDynamic")

    # Thời gian chạy của từng thuật toán với từng threads khác nhau
    infomationThreads(dataThreads[1], "AStarParallel Algorithms", "indianred", 0, "astar")
    infomationThreads(dataThreads[1], "ACOParallel Algorithms", "lightsalmon", 1,"aco")

    # So sánh thời gian chạy của 2 thuật toán AStar Parallel và ACO Parallel trên nhiều maze khác nhau
    compareDiffMazes(dataTimeMazeDy[1], "Time")

    # Tìm điểm giao của 2 đường thẳng
    findIntersection(dataTimeMaze[1], "Time")

main()


