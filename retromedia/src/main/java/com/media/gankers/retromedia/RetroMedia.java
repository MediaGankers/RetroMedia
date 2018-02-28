package com.media.gankers.retromedia;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.media.gankers.retromedia.define.WorkFlowType;
import com.media.gankers.retromedia.workflow.WorkFlow;

import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

/**
 * Created by gordon on 2/2/18.
 */

public class RetroMedia {

    private static final String WORK_FLOW_CAMERA = "WORK_FLOW_CAMERA";
    private static final String WORK_FLOW_FILE = "WORK_FLOW_FILE_%d";
    private static final String WORK_FLOW_NET = "WORK_FLOW_NET_%d";

    private RetroMedia() {
    }

    private static class Holder {
        private static RetroMedia sInstance = new RetroMedia();
    }

    private static RetroMedia getInternal() {
        return Holder.sInstance;
    }

    private Map<String, WorkFlow> mWorkFlows = new HashMap<>();

    private WorkFlow generateInternal(WorkFlowType type, String resPath) {
        WorkFlow finalFlow;
        switch (type) {
            case CAMERA:
            default:
                mWorkFlows.put(WORK_FLOW_CAMERA, finalFlow = new WorkFlow(type));
                break;
            case FILE:
                mWorkFlows.put(String.format(Locale.getDefault(), WORK_FLOW_FILE, resPath.hashCode()), finalFlow = new WorkFlow(type));
                break;
            case NETWORK:
                mWorkFlows.put(String.format(Locale.getDefault(), WORK_FLOW_NET, resPath.hashCode()), finalFlow = new WorkFlow(type));
                break;
        }
        return finalFlow;
    }

    public static WorkFlow fromCamera() {
        return getInternal().generateInternal(WorkFlowType.CAMERA, null);
    }

    public static WorkFlow fromFile(@NonNull String absolutePath) {
        return getInternal().generateInternal(WorkFlowType.FILE, absolutePath).res(absolutePath);
    }

    public static WorkFlow fromNetwork(@NonNull String remoteUrl) {
        return getInternal().generateInternal(WorkFlowType.NETWORK, remoteUrl).res(remoteUrl);
    }

    @Nullable
    public static WorkFlow cameraFlow() {
        return getInternal().mWorkFlows.get(WORK_FLOW_CAMERA);
    }
}
