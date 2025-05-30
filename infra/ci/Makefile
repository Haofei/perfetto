# Copyright (C) 2019 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include $(shell python3 config.py makefile)

override COMMON_DEPS := Makefile *.py
override GCE_LOCAL_STARTUP_SCRIPT := worker/gce-startup-script.sh
override SCRIPT_HASH := $(shell git hash-object ${GCE_LOCAL_STARTUP_SCRIPT} | cut -c 1-8)
override GCE_STARTUP_SCRIPT := gs://perfetto/ci/worker-startup-script/${SCRIPT_HASH}
BUILDER := docker

.PHONY: help
help:
	@echo "build:              Builds the worker and sandbox containers"
	@echo "build-worker:       Builds the worker container"
	@echo "build-sandbox:      Builds the sandbox container"
	@echo "push:               Pushes the containers to the registry"
	@echo "deploy-frontend:    Deploys and restarts the controller"
	@echo "stop-workers:       Stops the whole workers GCE instance group"
	@echo "start-workers:      Starts the whole workers GCE instance group"
	@echo "restart-workers:    Restarts the whole workers GCE instance group"

.PHONY: build
build: build-worker build-sandbox

.PHONY: build-worker
build-worker: .deps/${BUILDER}-worker

.PHONY: build-sandbox
build-sandbox: .deps/${BUILDER}-sandbox

.PHONY: push
push: build
	${BUILDER} push ${WORKER_IMG}
	${BUILDER} push ${SANDBOX_IMG}

.PHONY: clean
clean:
	rm -rf .deps

.deps/${BUILDER}-worker: worker/* ${COMMON_DEPS}
	mkdir -p worker/tmp
	cp -a config.py common_utils.py worker/tmp/
	${BUILDER} build --rm --force-rm -t ${WORKER_IMG} worker
	rm -rf worker/tmp/
	touch $@

.deps/${BUILDER}-sandbox: sandbox/* ${COMMON_DEPS}
	mkdir -p sandbox/tmp
	cp -a config.py common_utils.py sandbox/tmp/
	${BUILDER} build --rm --force-rm -t ${SANDBOX_IMG} sandbox
	rm -rf sandbox/tmp/
	touch $@

.deps/upload-startup-script: ${GCE_LOCAL_STARTUP_SCRIPT} ${COMMON_DEPS}
	gsutil -q cp -a public-read ${GCE_LOCAL_STARTUP_SCRIPT} ${GCE_STARTUP_SCRIPT}
	touch $@

.deps/gce-template: ${COMMON_DEPS} .deps/upload-startup-script
	gcloud compute --quiet --project=${PROJECT} \
		instance-templates delete --quiet ${GCE_TEMPLATE} || true
	gcloud compute --quiet --project=${PROJECT} \
		instance-templates create ${GCE_TEMPLATE} \
		--enable-nested-virtualization \
		--machine-type=${GCE_VM_TYPE} \
		--network=projects/${PROJECT}/global/networks/default \
		--network-tier=PREMIUM \
		--metadata='startup-script-url=${GCE_STARTUP_SCRIPT},num-workers=${NUM_WORKERS_PER_VM},sandbox-img=${SANDBOX_IMG},worker-img=${WORKER_IMG},google-logging-enabled=true,enable-oslogin=TRUE' \
		--maintenance-policy=MIGRATE \
		--service-account=gce-ci-worker@${PROJECT}.iam.gserviceaccount.com \
		--scopes=${GCE_SCOPES} \
		--image=projects/cos-cloud/global/images/cos-121-18867-90-23 \
		--boot-disk-size=100GB \
		--boot-disk-type=pd-ssd \
		--boot-disk-device-name=${GCE_TEMPLATE} \
		--local-ssd=interface=NVME \
		--local-ssd=interface=NVME
	touch $@

.PHONY: deploy-frontend
deploy-frontend:
	make -C frontend deploy

.PHONY: restart-workers
restart-workers: stop-workers start-workers

define start-workers-for-region
gcloud compute --project=${PROJECT} \
	instance-groups managed create ${GCE_GROUP_NAME}-$1 \
	--region=$1 \
	--base-instance-name=${GCE_GROUP_NAME}-$1 \
	--template=${GCE_TEMPLATE} \
	--size 0
gcloud compute --quiet --project=$(PROJECT) \
	instance-groups managed set-autoscaling ${GCE_GROUP_NAME}-$1 \
	--region=$1 \
	--min-num-replicas=${AUTOSCALER_MIN} \
	--max-num-replicas=${MAX_VMS_PER_REGION} \
	--cool-down-period=60 \
	--stackdriver-metric-filter="resource.type = \"global\"" \
	--update-stackdriver-metric="custom.googleapis.com/$(PROJECT)/ci_job_queue_len" \
	--stackdriver-metric-utilization-target=0.1 \
	--stackdriver-metric-utilization-target-type=gauge
endef

.PHONY: start-workers
start-workers: .deps/gce-template
	$(foreach region,$(GCE_REGIONS),$(call start-workers-for-region,$(region)))

define stop-workers-for-region
gcloud compute --quiet --project=${PROJECT} \
	instance-groups managed delete ${GCE_GROUP_NAME}-$1 --region=$1 || true
endef

.PHONY: stop-workers
stop-workers:
	$(foreach region,$(GCE_REGIONS),$(call stop-workers-for-region,$(region)))


.PHONY: autoscaler-debug
autoscaler-debug:
	gcloud compute instance-groups managed describe
		${GCE_GROUP_NAME}-${GCE_REGIONS} \
		--project=${PROJECT} \
		--region=${GCE_REGIONS}

# These are for testing only, start an individual VM. Use start-group for
# production.

.PHONY: stop-worker-for-testing
stop-worker-for-testing:
	gcloud compute --quiet \
		--project ${PROJECT} \
		instances delete ${GCE_VM_NAME} \
		--zone us-west1-c

.PHONY: start-worker-for-testing
start-worker-for-testing: .deps/gce-template
	gcloud compute --quiet \
		--project ${PROJECT} \
		instances create ${GCE_VM_NAME} \
		--zone us-west1-c \
		--source-instance-template=${GCE_TEMPLATE}

# Debugging client to make OAuth2 authenticated requests manually.
.PHONY: cli
cli:
	GOOGLE_APPLICATION_CREDENTIALS=test-credentials.json \
	python3 -i -c 'from common_utils import *; from config import *; \
		SCOPES += ["https://www.googleapis.com/auth/firebase.database", \
								"https://www.googleapis.com/auth/userinfo.email", \
								"https://www.googleapis.com/auth/datastore"]'
